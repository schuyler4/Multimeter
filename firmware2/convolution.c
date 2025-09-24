#include <stdint.h>
#include <stdlib.h>

float *convolve(float *h, float *x, uint16_t len_h, uint16_t len_x, uint16_t* len_y)
{
    uint16_t convolution_length = len_h+len_x-1; 
    float *y = (float*)calloc(convolution_length, sizeof(float));
    uint16_t i;

    for(i = 0; i < convolution_length; i++)
    {
        
    }
}