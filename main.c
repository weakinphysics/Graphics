#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <math.h>
#include "./src/rohan.h"


#define WIDTH 800
#define HEIGHT 600


static uint32_t pixels[HEIGHT*WIDTH];

void fill_rectangle(uint32_t *pixels, int width, int height, int centerX, int centerY, int fullWidth, int fullHeight, uint32_t color){
    assert((centerX - width/2 >= 0) && (centerX + ceil(width/2.0) - 1 < fullWidth));
    assert((centerY - height/2 >= 0) && (centerY + ceil(height/2.0) - 1 < fullHeight));
    int start_X = centerX - width/2;
    int start_Y = centerY - height/2;
    for(int dy = 0; dy < height; dy++){
        for(int dx = 0; dx < width; dx++){
            pixels[(start_Y + dy)*fullWidth + start_X + dx] = color;
        }
    } 
    return;
}

void fill_checkered_pattern(uint32_t *pixels, int boxCountH, int boxCountY, int centerX, int centerY, int width, int height, int fullWidth, int fullHeight){
    
}

int main(void){
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFFFF00FF);
    fill_rectangle(pixels, 800, 600, 400, 300, 800, 600, 0xFFFF0000);
    olivec_save(pixels, WIDTH, HEIGHT, "output.ppm");
    return 0;
}
