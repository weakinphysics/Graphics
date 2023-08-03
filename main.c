#include "./src/rohan.h"


#define WIDTH 800
#define HEIGHT 600 

static uint32_t pixels[HEIGHT*WIDTH];



int main(void){
    double a = 20;
    double b = 50;
    double resolution = (b-a)/5.0;
    printf("%lf\n", resolution); 
    int arrLen = (b-a)/resolution;
    double values[arrLen];
    perform_linear_interpolation(a, b, resolution, values, false);
    for(int i = 0; i < arrLen; i++){
        printf("%lf ", values[i]);
    }
    printf("\n");
    olivec_fill(pixels, WIDTH, HEIGHT, 0xFFFF0000);
    // fill_rectangle(pixels, 800, 600, 400, 300, 800, 600, 0xFFFFFFFF);
    // fill_circular_grid(pixels, 960, 540, 600, 10, 35, WIDTH, HEIGHT, 0xFFFF2020);
    // fill_checkered_pattern(pixels, 8, 8, 400, 300, 480, 480, 800, 600, 0xFFFF2020, 0xFF000000);
    // fill_circle(pixels, 400, 300, 50, WIDTH, HEIGHT, 0xFFFF2020);
    // draw_line(pixels, 1.0, 30.0, 1.0, 10.0, 0xFFFFFFFF, WIDTH, HEIGHT);
    // double points[6] = {20, 20, 80, 80, 60, 200};
    double points2[6] = {51, 65, 102, 260, 342, 343};
    // fill_triangle(pixels, points, 0xFFFFFFFF, WIDTH, HEIGHT);
    fill_triangle(pixels, points2, 0xFF00FFFF, WIDTH, HEIGHT);
    olivec_save(pixels, WIDTH, HEIGHT, "output.ppm");
    return 0;
}
