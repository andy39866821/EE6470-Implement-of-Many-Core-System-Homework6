#include <string.h>
#include "stdio.h"

//Filter ACC
static char* const FILTER_START_ADDR = reinterpret_cast<char* const>(0x73000000);
static char* const FILTER_READ_ADDR  = reinterpret_cast<char* const>(0x73000004);

// DMA 
static volatile uint32_t * const DMA_SRC_ADDR  = (uint32_t * const)0x70000000;
static volatile uint32_t * const DMA_DST_ADDR  = (uint32_t * const)0x70000004;
static volatile uint32_t * const DMA_LEN_ADDR  = (uint32_t * const)0x70000008;
static volatile uint32_t * const DMA_OP_ADDR   = (uint32_t * const)0x7000000C;
static volatile uint32_t * const DMA_STAT_ADDR = (uint32_t * const)0x70000010;
static const uint32_t DMA_OP_MEMCPY = 1;

bool _is_using_dma = true;

void write_data_to_ACC(char* ADDR, unsigned char* buffer, int len){
    if(_is_using_dma){  
        // Using DMA 
        *DMA_SRC_ADDR = (uint32_t)(buffer);
        *DMA_DST_ADDR = (uint32_t)(ADDR);
        *DMA_LEN_ADDR = len;
        *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    }else{
        // Directly Send
        memcpy(ADDR, buffer, sizeof(unsigned char)*len);
    }
}
void read_data_from_ACC(char* ADDR, unsigned char* buffer, int len){
    if(_is_using_dma){
        // Using DMA 
        *DMA_SRC_ADDR = (uint32_t)(ADDR);
        *DMA_DST_ADDR = (uint32_t)(buffer);
        *DMA_LEN_ADDR = len;
        *DMA_OP_ADDR  = DMA_OP_MEMCPY;
    }else{
        // Directly Read
        memcpy(buffer, ADDR, sizeof(unsigned char)*len);
    }
}
bool test_result(unsigned char* golden, unsigned char* buffer, int len, int i, int j){
}
int main() {
    #include "input_bitmap.h" //included here to avoid compiler issue of not initializing global arrays
	#include "output_bitmap.h" //included here to avoid compiler issue of not initializing global arrays
    bool pass = true;
    unsigned int rgb_raw_data_offset = input_rgb_raw_data_offset;
	unsigned int width = input_bmp_width;
	unsigned int height = input_bmp_height;
	unsigned int bytes_per_pixel = input_bmp_byte_per_pixel;
	unsigned char* source_bitmap = input_bmp;
    printf("======================================\n");
    printf("\t  Reading from source file\n");
    printf("======================================\n");
	printf(" input_rgb_raw_data_offset\t= %d\n", rgb_raw_data_offset);
	printf(" width\t\t\t\t= %d\n", width);
	printf(" height\t\t\t\t= %d\n", height);
	printf(" bytes_per_pixel\t\t= %d\n",bytes_per_pixel);
    printf("======================================\n");

    unsigned char  buffer[4] = {0};
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            for(int y = -1; y <= 1; y ++) {
                for(int x = -1; x <= 1; x++) {
                    if(0 <= i + y && i + y < height && 0 <= j + x && j + x < width){
                        buffer[0] = *(source_bitmap + bytes_per_pixel * ((i + y) * width + (j + x)) + 2);
                        buffer[1] = *(source_bitmap + bytes_per_pixel * ((i + y) * width + (j + x)) + 1);
                        buffer[2] = *(source_bitmap + bytes_per_pixel * ((i + y) * width + (j + x)) + 0);
                        buffer[3] = 0;
                    }
                    else{
                        buffer[0] = 0;
                        buffer[1] = 0;
                        buffer[2] = 0;
                        buffer[3] = 0;
                    }
                    write_data_to_ACC(FILTER_START_ADDR, buffer, 4);
                }
            }
            read_data_from_ACC(FILTER_READ_ADDR, buffer, 4);
            
            unsigned char R = output_bmp[bytes_per_pixel * (i * width + j) + 2];
            unsigned char G = output_bmp[bytes_per_pixel * (i * width + j) + 1];
            unsigned char B = output_bmp[bytes_per_pixel * (i * width + j) + 0];
            
            if(R != buffer[0]){
                printf("[ERROR] R: i:%d, j:%d, result:%d, correct:%d\n", i, j, buffer[2], R);
                pass = false;
            }
            if(G != buffer[1]){
                printf("[ERROR] G: i:%d, j:%d, result:%d, correct:%d\n", i, j, buffer[1], G);
                pass = false;
            }
            if(B != buffer[2]){
                printf("[ERROR] B: i:%d, j:%d, result:%d, correct:%d\n", i, j, buffer[0], B);
                pass = false;
            }
        }
        printf("Current: %d\n", i);
    }
    printf("----------------Simulation Finished----------------\n");
    if(pass == false){
        printf("========> Failed!\n");
    }
    else {
        printf("========> All PASS\n");
    }
}
