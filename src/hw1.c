#include "hw1.h"
#include <stdio.h>

unsigned Source_Address, Destination_Address, Source_Port, Destination_Port, Fragment_Offset, packet_length, MHP, Check_Sum, Compression_Scheme, Traffic_Class;

void print_packet_sf(unsigned char packet[])
{
    //Source Address
    Source_Address =  (packet[0] << 20) | (packet[1] << 12) | (packet[2] << 4) | packet[3] >> 4;
    printf("Source Address: %u\n", Source_Address);

    //Destination Address
    Destination_Address = ((packet[3] & 0X0F) << 24) | (packet[4] << 16) | (packet[5] << 8) | packet[6];
    printf("Destination Address: %u\n", Destination_Address);

    //Source Port
    Source_Port = (packet[7] >> 4) ;
    printf("Source Port: %u\n", Source_Port ); 

    //Destination Port
    Destination_Port = (packet[7] & 0X0F);
    printf("Destination Port: %u\n", Destination_Port);

    //Fragment Offset
    Fragment_Offset = (packet[8]  << 6) | (packet[9] >> 2);
    printf("Fragment Offset: %u\n", Fragment_Offset );

    //Packet Length
    packet_length = (((packet[9]&0x03) << 12) | packet[10] << 4 | packet[11] >> 4);
    printf("Packet Length: %u\n", packet_length );

    //Maximum Hop Count 
    MHP = ((packet[11] & 0x0F) << 1) | (packet[12]>>7);
    printf("Maximum Hop Count: %u\n", MHP);

    //Checksum
    Check_Sum = ((packet[12] & 0X7F) << 16) | (packet[13] << 8) | packet[14];
    printf("Checksum: %u\n", Check_Sum);

    //Compression Scheme
    Compression_Scheme= (packet[15] >> 6);
    printf("Compression Scheme: %u\n",Compression_Scheme );

    //Traffic Class
    Traffic_Class = packet[15] & 0X3F;
    printf("Traffic Class: %u\n", Traffic_Class);
    int payload_length = packet_length - 16;
    //Payload
    printf("Payload: ");
    for (int i = 16; i < packet_length ; i += 4) {
        int payload_chunk = (packet[i] << 24) | (packet[i + 1] << 16) | (packet[i + 2] << 8) | packet[i + 3];
        if(i == packet_length-4 ){
            printf("%d", payload_chunk);
            break;
        }
        else{
            printf("%d ", payload_chunk);
        }
    }
    printf("\n");
}

unsigned int compute_checksum_sf(unsigned char packet[])
{
    Source_Address =  (packet[0] << 20) | (packet[1] << 12) | (packet[2] << 4) | packet[3] >> 4;
    Destination_Address = ((packet[3] & 0X0F) << 24) | (packet[4] << 16) | (packet[5] << 8) | packet[6];
    Source_Port = (packet[7] >> 4) ;
    Destination_Port = (packet[7] & 0X0F);
    Fragment_Offset = (packet[8]  << 6) | (packet[9] >> 2);
    packet_length = (((packet[9]&0x03) << 12) | packet[10] << 4 | packet[11] >> 4);
    MHP = ((packet[11] & 0x0F) << 1) | (packet[12]>>7);
    Compression_Scheme= (packet[15] >> 6);
    Traffic_Class = packet[15] & 0X3F;
    unsigned int sum = Source_Address + Destination_Address + Source_Port + Destination_Port + Fragment_Offset + packet_length + MHP + Compression_Scheme + Traffic_Class;

    for (int i = 16; i < packet_length ; i += 4) {
        int payload_chunk = (packet[i] << 24) | (packet[i + 1] << 16) | (packet[i + 2] << 8) | packet[i + 3];
        sum += abs(payload_chunk);
    }

    sum = sum%(( 1 << 23) - 1);

    return sum;

}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) {
    unsigned int reconstructed_count = 0;

    for (unsigned int i = 0; i < packets_len; i++) {

        Source_Address =  (packets[i][0] << 20) | (packets[i][1] << 12) | (packets[i][2] << 4) | packets[i][3] >> 4;
        Destination_Address = ((packets[i][3] & 0X0F) << 24) | (packets[i][4] << 16) | (packets[i][5] << 8) | packets[i][6];
        Source_Port = (packets[i][7] >> 4) ;
        Destination_Port = (packets[i][7] & 0X0F);
        Fragment_Offset = (packets[i][8]  << 6) | (packets[i][9] >> 2);
        packet_length = (((packets[i][9]&0x03) << 12) | packets[i][10] << 4 | packets[i][11] >> 4);
        MHP = ((packets[i][11] & 0x0F) << 1) | (packets[i][12]>>7);
        Compression_Scheme= (packets[i][15] >> 6);
        Traffic_Class = packets[i][15] & 0X3F;
        Check_Sum = ((packets[i][12] & 0X7F) << 16) | (packets[i][13] << 8) | packets[i][14];
        
        if (compute_checksum_sf(packets[i]) !=Check_Sum) 
        {
            continue;
        }
        else
        {
            
        unsigned int payload_size = packet_length - 16;
        unsigned int fragment_offset = Fragment_Offset / 4;

        unsigned int start_index = fragment_offset;

       
        for (unsigned int j = 16, k = start_index; j < 16 + payload_size; j += 4, k++) {
            if (k < array_len) {
                array[k] = (packets[i][j] << 24) | (packets[i][j + 1] << 16) | (packets[i][j + 2] << 8) | packets[i][j + 3];
                reconstructed_count++;
            } else {
                break;
            }
        }
        }
    }
    return reconstructed_count;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class)
{
       unsigned int num_packets = (array_len + max_payload - 1) / max_payload; 

    for (unsigned int i = 0; i < packets_len && i < num_packets; i++) {        
        unsigned int payload_size = (i == num_packets - 1) ? (array_len % max_payload) : max_payload;

        unsigned int fragment_offset = i * max_payload * sizeof(int);
        unsigned int packet_length = payload_size * sizeof(int) + 16;
        unsigned int checksum = 0; 

        packets[i] = malloc(packet_length);

        packets[i][0] = (src_addr >> 20) & 0xFF;
        packets[i][1] = (src_addr >> 12) & 0xFF;
        packets[i][2] = (src_addr >> 4) & 0xFF;
        packets[i][3] = ((src_addr & 0xF) << 4) | ((dest_addr >> 24) & 0xFF);
        packets[i][4] = (dest_addr >> 16) & 0xFF;
        packets[i][5] = (dest_addr >> 8) & 0xFF;
        packets[i][6] = dest_addr & 0xFF;
        packets[i][7] = ((src_port << 4) & 0xF0) | (dest_port & 0xF);
        packets[i][8] = (fragment_offset >> 6) & 0xFF;
        packets[i][9] = ((fragment_offset & 0x3F) << 2) | ((packet_length >> 12) & 0x3);
        packets[i][10] = (packet_length >> 4) & 0xFF;
        packets[i][11] = ((packet_length & 0xF) << 4) | ((maximum_hop_count >> 1) & 0xF);
        packets[i][12] = ((maximum_hop_count & 0x1) << 7) | ((checksum >> 16) & 0x7F);
        packets[i][13] = (checksum >> 8) & 0xFF;
        packets[i][14] = checksum & 0xFF;
        packets[i][15] = ((compression_scheme << 6) & 0xC0) | (traffic_class & 0x3F);

        for (unsigned int j = 0, k = 16; j < payload_size; j++, k += sizeof(int)) {
            packets[i][k] = (array[i * max_payload + j] >> 24) & 0xFF;
            packets[i][k + 1] = (array[i * max_payload + j] >> 16) & 0xFF;
            packets[i][k + 2] = (array[i * max_payload + j] >> 8) & 0xFF;
            packets[i][k + 3] = array[i * max_payload + j] & 0xFF;
        }
    }

    return num_packets;
}
