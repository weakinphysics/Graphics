#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define COLOR_RANGE 255
#define PIXEL_AT(m, i, j) (m)[(i) * ((m).stride) + (j)]

void make_ppm_header(FILE* fp, int width, int height){
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d %d\n",width, height, COLOR_RANGE);
}

void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color){
    for(size_t i = 0; i < height*width; i++){
        pixels[i] = color; 
    }
}


int terminateFileWrite(FILE* fp, int status){
    if(status == -1){
        if(fp != NULL) fclose(fp);
        return -1;
    }
    if(fp != NULL){
        fclose(fp);
        return 1;
    }
    return status;
}

int olivec_save(uint32_t *pixels, int width, int height, const char* filePath){
    // the .ppm format is one of the easiest to handle(read/write) since it does not have any compression/conversion, and data can straight up be written in binary/ascii

    FILE* filePointer = NULL;
    filePointer = fopen(filePath, "wb");
    if(filePointer == NULL) return terminateFileWrite(filePointer, -1);
    make_ppm_header(filePointer, width, height);
    for(int i = 0; i < width*height; i++){
        uint32_t pixel = pixels[i];
        uint8_t bytes[3] = {
            (pixel >> (8*0))&0xFF,
            (pixel >> (8*1))&0xFF,
            (pixel >> (8*2))&0xFF
        };
        fwrite(bytes, sizeof(bytes), 1, filePointer);
        if(ferror(filePointer)) return terminateFileWrite(filePointer, -1);
    }
    return terminateFileWrite(filePointer, 1);
}

