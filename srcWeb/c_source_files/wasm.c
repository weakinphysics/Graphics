#include "rohanWeb.c"

#define WIDTH 800
#define HEIGHT 600

uint32_t pixels[WIDTH*HEIGHT];

uint32_t* render(void){
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFFFF0000);
    int x1 = WIDTH/2;
    int y1 = HEIGHT/8;
    int x2 = WIDTH/8;
    int y2 = HEIGHT/2;
    int x3 = (WIDTH*7)/8;
    int y3 = (HEIGHT*7)/8;
    double points[6];
    points[0] = x1;
    points[1] = y1;
    points[2] = x2;
    points[3] = y2;
    points[4] = x3;
    points[5] = y3;
    fill_triangle(pixels, points, 0xFF00FF00, WIDTH, HEIGHT);
    return pixels;
}