#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#define COLOR_RANGE 255
#define PIXEL_AT(m, i, j) (m)[(i) * ((m).stride) + (j)]


void make_ppm_header(FILE* fp, int width, int height);
void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color);
void fill_rectangle(uint32_t *pixels, int width, int height, int centerX, int centerY, int fullWidth, int fullHeight, uint32_t color);
void fill_circle(uint32_t *pixels, int cx, int cy, double radius, int fullWidth, int fullHeight, uint32_t color);
void fill_checkered_pattern(uint32_t *pixels, int boxCountH, int boxCountV, int centerX, int centerY, int width, int height, int fullWidth, int fullHeight, uint32_t color1, uint32_t color2);
void  fill_circular_grid(uint32_t *pixels, int gridCenterX, int gridCenterY, int containerEdgeLength, int minRadius, int maxRadius, int fullWidth, int fullHeight, uint32_t color);
int terminateFileWrite(FILE* fp, int status);
int olivec_save(uint32_t *pixels, int width, int height, const char* filePath);


void make_ppm_header(FILE* fp, int width, int height){
    fprintf(fp, "P6\n");
    fprintf(fp, "%d %d %d\n",width, height, COLOR_RANGE);
}

void swaps(double *a, double *b){
    double t = *a;
    *a = *b;
    *b = t;
}

void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color){
    for(size_t i = 0; i < height*width; i++){
        pixels[i] = color; 
    }
}

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

void sortTheStuff(double *ptr){
    if(ptr[1] > ptr[3]){
        swaps(&(ptr[0]), &(ptr[2]));
        swaps(&(ptr[1]), &(ptr[3]));
    }
    if(ptr[3] > ptr[5]){
        swaps(&(ptr[2]), &(ptr[4]));
        swaps(&(ptr[3]), &(ptr[5]));
    }
    printf("Sorted!\n");
}

void draw_line(uint32_t *pixels, double x1, double y1, double x2, double y2, uint32_t lineColor, int fullWidth, int fullHeight){
    assert((x1 >= 0 && x1 < fullWidth) && (y1 >= 0 && y1 < fullHeight));
    assert((x2 >= 0 && x2 < fullWidth) && (y2 >= 0 && y2 < fullHeight));
    if(x1 > x2){
        swaps(&x1, &x2);
        swaps(&y2, &y1);
    }
    double delta_x = x2 - x1;
    double delta_y = y2 - y1;
    double constant, slope;
    if(delta_x){
        slope = delta_y/delta_x;
        constant =  y1 - slope*x1;      
        int y_prev = y1;
        for(int i = (int)x1; i <= (int)x2; i++){
            int y_new = (int)(slope*i + constant);
            pixels[y_new*fullWidth + i] = lineColor;
            while(y_prev < y_new){
                pixels[y_prev*fullWidth + i] = lineColor;
                y_prev++;
            }
            while(y_prev > y_new){
                pixels[y_prev*fullWidth + i] = lineColor;
                y_prev--;
            }
        }
    }
    else{
        if(y1 > y2) swaps(&y1, &y2);
        for(int i = y1; i < y2; i++){
            pixels[i*fullWidth + (int)x1] = lineColor;
        }
    }
}


void fill_triangle(uint32_t *pixels, double *points, uint32_t color){
    // one way of going about this is perhaps drawing borders and then filling everything within the borders. A triangle by its
    // very nature is convex. I suppose this property of triangles makes it suitable to draw other shapes; 


    //another way would be to draw the lines between all points of two lines of a triangle and then choose the appropriate combination of lines to 
    //fill the space between in 

    void sortTheStuff(points);
    
}

void fill_circle(uint32_t *pixels, int cx, int cy, double radius, int fullWidth, int fullHeight, uint32_t color){
    fill_rectangle(pixels, 2*radius , 2*radius , cx, cy, fullWidth, fullHeight, 0xFF000000);
    int start_X = cx - radius;

    int start_Y = cy - radius;

    //apply circular mask
    for(int dy = start_Y; dy < start_Y + 2*radius; dy++){
        for(int dx = start_X; dx < start_X + 2*radius; dx++){
            if(radius*radius >= (dx - cx)*(dx - cx) + (dy - cy)*(dy - cy)){
                // printf("%d %d\n", dx, dy);
                pixels[dy*fullWidth + dx] = color;
            }
        }
    }
    return;
}

void fill_checkered_pattern(uint32_t *pixels, int boxCountH, int boxCountV, int centerX, int centerY, int width, int height, int fullWidth, int fullHeight, uint32_t color1, uint32_t color2){
    assert((centerX - width/2) >= 0 && (centerX + ceil(width/2.0) - 1 < fullWidth));
    assert(centerY - height/2 >= 0 && (centerY  + ceil(height/2.0) - 1 < fullHeight));
    int containerStart_X = centerX - width/2;
    int containerStart_Y = centerY - height/2;
    int individualBoxWidth = width/boxCountH;
    int individualBoxHeight = height/boxCountV;
    fill_rectangle(pixels, width, height, centerX, centerY, fullWidth, fullHeight, 0xFFFFFFFF);
    int cx,cy;
    uint32_t color;
    for(int i = 0; i < boxCountV; i++){
        for(int j = 0; j < boxCountH; j++){
            if((i^j)&1) color = color1;
            else color = color2;
            cx =  containerStart_X + j*individualBoxWidth + (individualBoxWidth/2);
            cy =  containerStart_Y + i*individualBoxHeight + (individualBoxHeight/2);
            fill_rectangle(pixels, individualBoxWidth, individualBoxHeight, cx, cy, fullWidth, fullHeight, color);
        }
    }
}

void perform_linear_interpolation(double a, double b, double resolution, double* values, bool includeLast){
    int listSize = (b-a)/resolution;
    int i = 0;
    while(i < listSize){
        values[i] = a;
        i++;
        a += resolution;
    }
    return;
}

void  fill_circular_grid(uint32_t *pixels, int gridCenterX, int gridCenterY, int containerEdgeLength, int minRadius, int maxRadius, int fullWidth, int fullHeight, uint32_t color){
    //much like fill_checkered_pattern we merely need to ascertain the circle centers and then we may call the fill_circle method from each center;
    int boxCount = 5*5;
    double resolution = (maxRadius - minRadius)/((double)boxCount);
    //the container in this case is a square
    assert((gridCenterX - containerEdgeLength/2) >= 0 && (gridCenterX + ceil(containerEdgeLength/2.0) - 1 < fullWidth));
    assert((gridCenterY - containerEdgeLength/2 >= 0) && (gridCenterY + ceil(containerEdgeLength/2.0) - 1 < fullHeight));
    int individualBoxEdge = containerEdgeLength/5;
    fill_rectangle(pixels, containerEdgeLength, containerEdgeLength, gridCenterX, gridCenterY, fullWidth, fullHeight, 0xFF000000);
    int containerStartX = gridCenterX - containerEdgeLength/2;
    int containerStartY = gridCenterY - containerEdgeLength/2;
    double radiusValues[boxCount];
    perform_linear_interpolation((double)minRadius, (double)maxRadius, resolution, radiusValues, false);
    
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            int cx = containerStartX + j*individualBoxEdge + individualBoxEdge/2;
            int cy = containerStartY + i*individualBoxEdge + individualBoxEdge/2;
            fill_circle(pixels, cx, cy, radiusValues[i*5 + j], fullWidth, fullHeight, color);
        }
    }
    return;
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



