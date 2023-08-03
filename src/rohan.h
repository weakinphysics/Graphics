// this is the core rohan library, that carries methods for manipulating rohan objects 
// if you want to run stuff in a browser, check out rohanWeb, which is c code, ported to webAssembly
// if you want to run a native/desktop application, go to rohanNative, which supports diplaying stuff using SDL windows.
// this process is atleast a 100 times slower than running the same code using openGL, but is much easier to understand(this runs on CPU)



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
#define PI 3.14
void make_ppm_header(FILE* fp, int width, int height);
void swaps(double *a, double *b);
void sorting_for_triangle_render(double *ptr);
void draw_line(uint32_t *pixels, double x1, double y1, double x2, double y2, uint32_t lineColor, int fullWidth, int fullHeight);
bool areCollinear(double *points);
void perform_linear_interpolation(double a, double b, double resolution, double* values, bool includeLast);
void rotate_about_origin(int *x, int *y, int dtheta, int fullWidth, int fullHeight);
void olivec_fill(uint32_t *pixels, size_t width, size_t height, uint32_t color);
void fill_rectangle(uint32_t *pixels, int width, int height, int centerX, int centerY, int fullWidth, int fullHeight, uint32_t color);
void fill_circle(uint32_t *pixels, int cx, int cy, double radius, int fullWidth, int fullHeight, uint32_t color);
void fill_checkered_pattern(uint32_t *pixels, int boxCountH, int boxCountV, int centerX, int centerY, int width, int height, int fullWidth, int fullHeight, uint32_t color1, uint32_t color2);
void fill_circular_grid(uint32_t *pixels, int gridCenterX, int gridCenterY, int containerEdgeLength, int minRadius, int maxRadius, int fullWidth, int fullHeight, uint32_t color);
void fill_triangle(uint32_t *pixels, double *points, uint32_t color, int fullWidth, int fullHeight);
int terminateFileWrite(FILE* fp, int status);
int olivec_save(uint32_t *pixels, int width, int height, const char* filePath);


typedef struct{
    uint8_t red_channel;
    uint8_t blue_channel;
    uint8_t green_channel;
    uint8_t alpha;
} Composition;

typedef struct{
    int x1;
    int y1;
    int width; 
    int height;
} Region;