#include <stdio.h>
#include "hw1.h"

int main() {
    #include "reconstruct_array_sf_args02.h"
    unsigned int elements_written = reconstruct_array_sf((unsigned char **)packets, sizeof(packets)/sizeof(packets[0]), 
        reconstructed_array, sizeof(reconstructed_array) / sizeof(reconstructed_array[0]));
    (void)elements_written;
    (void)expected_num_elements;
	(void)expected_array;
    return 0;
}

// int main() {
//     unsigned char packet[] = {
// 			0x01,0xd2,0x08,0xa0,0xb4,0x11,0xaa,0xcd,0x00,0x00,0x01,0xca,0xde,0xad,0xb1,0xf3,0x00,0x84,0x5f,0xed,0xff,0xff,0x66,0x8f,0x05,0x88,0x81,0x92,
// 		};
//     print_packet_sf(packet);
//     return 0;
// }
