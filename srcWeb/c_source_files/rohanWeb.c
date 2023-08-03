#include "rohanWeb.h"

///////////////////////////////////// ATTENTION ////////////////////////////////////////
/*
    This is a toned down version of the native rohan.h, since we won't be using the wasi standardized wasm(therefore no linking against anything posix, yes I mean you, libc),
    and therefore, any development here must be preceeded by development on the native source, since print statements are not available. Another drawback is the lack of dynamically allocated memory.
    This can all be rectified by the use of emscripten's sdk, however that adds a ton of bloat, and also makes development tricky, since most of the generated source files are at the moment, an 
    enigma to me. We will therefore use clang to generate a simple, light weight wasm file, and display the result in a canvas, no webgl or other fancy stuff. This shit runs on CPU!
*/

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

Composition segments(uint32_t color){
    Composition comp;
    comp.red_channel = (color>>8*0)&0xFF;
    comp.blue_channel = (color>>8*1)&0xFF;
    comp.green_channel = (color>>8*2)&0xFF;
    comp.alpha = (color>>8*3)&0xFF;
    return comp;
}



// void rotate_vector(int *x, int *y, double dtheta, int fullWidth, int fullHeight){
//     // dtheta must be in radians
//     double x_o = *x - fullWidth/2;
//     double y_o = *y - fullHeight/2;
//     double magnitude = sqrt(x_o*x_o + y_o*y_o);
//     double angle = atan2(y_o, x_o) + dtheta;
//     *x = (int)(magnitude*cos(angle));
//     *y = (int)(magnitude*sin(angle));
// }

void blend_alphas(uint32_t pixels, int fullWidth, int fullHeight){

}

void test_alpha_blending(uint32_t *pixels, int fullWidth, int fullHeight, uint32_t color1, uint32_t color2){
    int myWidth = fullWidth*0.75;
    int myHeight = fullHeight*0.75;
    int c1x = myWidth/2;
    int c1y = myHeight/2;
    int c2x = fullWidth - myWidth/2 - 1;
    int c2y = fullHeight - myHeight/2 -1;
    fill_rectangle(pixels, myWidth, myHeight, c1x, c1y, fullWidth, fullHeight, color1);
    fill_rectangle(pixels, myWidth, myHeight, c2x, c2y, fullWidth, fullHeight, color2);

    return;
 }
void fill_rectangle(uint32_t *pixels, int width, int height, int centerX, int centerY, int fullWidth, int fullHeight, uint32_t color){
    // bool chck = true;
    // if(!((centerX - width/2 >= 0) && (centerX + ceil(width/2.0) - 1 < fullWidth))) chck = false;
    // if(!((centerY - height/2 >= 0) && (centerY + ceil(height/2.0) - 1 < fullHeight))) chck = false;
    // if(!chck) return; // convert this into throwing an error instead of simply returning
    int start_X = centerX - width/2;
    int start_Y = centerY - height/2;
    for(int dy = 0; dy < height; dy++){
        for(int dx = 0; dx < width; dx++){
            pixels[(start_Y + dy)*fullWidth + start_X + dx] = color;
        }
    } 
    return;
}

void sort_for_triangle_render(double *ptr){
    for(int i = 1; i < 6; i+=2){
        for(int j = i+2; j < 6; j+=2){
            if(ptr[i] > ptr[j]){
                swaps(&(ptr[i]), &(ptr[j]));
                swaps(&(ptr[i-1]), &(ptr[j-1]));
            }
        }
    }
    
}

void draw_line(uint32_t *pixels, double x1, double y1, double x2, double y2, uint32_t lineColor, int fullWidth, int fullHeight){
    // assert((x1 >= 0 && x1 < fullWidth) && (y1 >= 0 && y1 < fullHeight));
    // assert((x2 >= 0 && x2 < fullWidth) && (y2 >= 0 && y2 < fullHeight));
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

bool areCollinear(double *points){
    if(points[2] == points[0]){
        if(points[4] == points[2]) return true;
        return false;
    }
    if(points[1] == points[3]){
        if(points[3] == points[5]) return true;
        return false;
    }
    double slope1 = (points[3] - points[1])/(points[2] - points[0]);
    double slope2 = (points[5] - points[3])/(points[4] - points[2]);
    return (slope1 == slope2);
}

void fill_triangle(uint32_t *pixels, double *points, uint32_t color, int fullWidth, int fullHeight){
    // one way of going about this is perhaps drawing borders and then filling everything within the borders. A triangle by its
    // very nature is convex. I suppose this property of triangles makes it suitable to draw other shapes; 


    //another way would be to draw the lines between all points of two lines of a triangle and then choose the appropriate combination of lines to 
    //fill the space between in 
    sort_for_triangle_render(points);
    if(areCollinear(points)){
        draw_line(pixels, points[0], points[1], points[4], points[5], color, fullWidth, fullHeight);
        return;
    } 
    double s2Inv = (points[4] - points[0])/(points[5] - points[1]);
    double d2 = points[4] - points[5]*s2Inv;
    // much like we calculate y from x using the slope-intercept form, we can calculate the opposite as well.
    if(points[3] != points[1]){
        double s1Inv = (points[2] - points[0])/(points[3] - points[1]);
        double d1 = points[2] - points[3]*s1Inv;
        for(int y = points[1]; y < points[3]; y++){
            int x1 = y*s1Inv + d1;
            int x2 = y*s2Inv + d2;
            draw_line(pixels, (double) x1, (double)y, (double)x2, (double)y, color, fullWidth, fullHeight);
        }
    }
    else draw_line(pixels, points[0], points[1], points[2], points[3], color, fullWidth, fullHeight);
    if(points[5] != points[3]){
        double s3Inv = (points[4] - points[2])/(points[5] - points[3]);
        double d3 = points[4] - points[5]*s3Inv;
        for(int y = points[3]; y <= points[5]; y++){
            int x2 = y*s2Inv + d2;
            int x3 = y*s3Inv + d3;
            draw_line(pixels, (double) x3, (double)y, (double)x2, (double)y, color, fullWidth, fullHeight);
        }  
    }
    return;
}

void fill_circle(uint32_t *pixels, int cx, int cy, double radius, int fullWidth, int fullHeight, uint32_t color){
    fill_rectangle(pixels, 2*radius , 2*radius , cx, cy, fullWidth, fullHeight, 0xFF000000);
    int start_X = cx - radius;

    int start_Y = cy - radius;

    //apply circular mask
    for(int dy = start_Y; dy < start_Y + 2*radius; dy++){
        for(int dx = start_X; dx < start_X + 2*radius; dx++){
            if(radius*radius >= (dx - cx)*(dx - cx) + (dy - cy)*(dy - cy)){
                pixels[dy*fullWidth + dx] = color;
            }
        }
    }
    return;
}

void fill_checkered_pattern(uint32_t *pixels, int boxCountH, int boxCountV, int centerX, int centerY, int width, int height, int fullWidth, int fullHeight, uint32_t color1, uint32_t color2){
    // assert((centerX - width/2) >= 0 && (centerX + ceil(width/2.0) - 1 < fullWidth));
    // assert(centerY - height/2 >= 0 && (centerY  + ceil(height/2.0) - 1 < fullHeight));
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
    // assert((gridCenterX - containerEdgeLength/2) >= 0 && (gridCenterX + ceil(containerEdgeLength/2.0) - 1 < fullWidth));
    // assert((gridCenterY - containerEdgeLength/2 >= 0) && (gridCenterY + ceil(containerEdgeLength/2.0) - 1 < fullHeight));
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







