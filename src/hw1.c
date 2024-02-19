#include "hw1.h"
#include <stdio.h>

void print_packet_sf(unsigned char packet[])
{
    //Source Address
    printf("Source Address: %u\n", (packet[0] << 20) | (packet[1] << 12) | (packet[2] << 4) | packet[3] >> 4);

    //Destination Address
    printf("Destination Address: %u\n", ((packet[3] & 0X0F) << 24) | (packet[4] << 16) | (packet[5] << 8) | packet[6]);

    //Source Port
    printf("Source Port: %u\n", (packet[7] >> 4) ); 

    //Destination Port
    printf("Destination Port: %u\n", packet[7] & 0X0F);

    //Fragment Offset
    printf("Fragment Offset: %u\n", (packet[8]  << 6) | packet[9] >> 6);

    //Packet Length
    unsigned packet_length = (packet[9] << 12 | packet[10] << 4 | packet[11] >> 4);
    printf("Packet Length: %u\n", packet_length );

    //Maximum Hop Count                                          
    printf("Maximum Hop Count: %u\n", ((packet[11] & 0x0F) << 1) | (packet[12]>>7));

    //Checksum
    printf("Checksum: %u\n", ((packet[12] & 0X7F) << 16) | (packet[13] << 8) | packet[14]);

    //Compression Scheme
    printf("Compression Scheme: %u\n", (packet[15] >> 6));

    //Traffic Class
    printf("Traffic Class: %u\n", packet[15] & 0X3F);

    //Payload
    printf("Payload: ");
    for (int i = 16; i < packet_length ; i += 4) {
        int payload_chunk = (packet[i] << 24) | (packet[i + 1] << 16) | (packet[i + 2] << 8) | packet[i + 3];
        if(i==packet_length-1){
            printf("%d", payload_chunk);
        }
        else{
            printf("%d ", payload_chunk);
        }
    }
    printf("\n");
}

unsigned int compute_checksum_sf(unsigned char packet[])
{
    (void)packet;
    return -1;
}

unsigned int reconstruct_array_sf(unsigned char *packets[], unsigned int packets_len, int *array, unsigned int array_len) {
    (void)packets;
    (void)packets_len;
    (void)array;
    (void)array_len;
    return -1;
}

unsigned int packetize_array_sf(int *array, unsigned int array_len, unsigned char *packets[], unsigned int packets_len,
                          unsigned int max_payload, unsigned int src_addr, unsigned int dest_addr,
                          unsigned int src_port, unsigned int dest_port, unsigned int maximum_hop_count,
                          unsigned int compression_scheme, unsigned int traffic_class)
{
    (void)array;
    (void)array_len;
    (void)packets;
    (void)packets_len;
    (void)max_payload;
    (void)src_addr;
    (void)dest_addr;
    (void)src_port;
    (void)dest_port;
    (void)maximum_hop_count;
    (void)compression_scheme;
    (void)traffic_class;
    return -1;
}
