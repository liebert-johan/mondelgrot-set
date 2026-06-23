
#include <TXLib.h>

#include <stdio.h>



int main(){
    int width = 800;
    int height = 600;
    double zoom = 200;
    double offset_x = -2.5;
    double offset_y = 1.25;
    double step = 20;

    int n_max = 256;
    double r2_max = 4;

    txCreateWindow(800, 600);
    txClearConsole();

    while (!GetAsyncKeyState(VK_ESCAPE)) {

        if (GetAsyncKeyState(VK_OEM_PLUS)) {
    double old_zoom = zoom;
    zoom *= 1.05;
    offset_x += (width  / 2.0) / old_zoom - (width  / 2.0) / zoom;
    offset_y += (height / 2.0) / old_zoom - (height / 2.0) / zoom;
    }

    if (GetAsyncKeyState(VK_OEM_MINUS)) {
        double old_zoom = zoom;
        zoom /= 1.05;
        offset_x += (width  / 2.0) / old_zoom - (width  / 2.0) / zoom;
        offset_y += (height / 2.0) / old_zoom - (height / 2.0) / zoom;
    }
        if (GetAsyncKeyState(VK_LEFT))  offset_x -= step / zoom;
        if (GetAsyncKeyState(VK_RIGHT)) offset_x += step / zoom;
        if (GetAsyncKeyState(VK_UP))    offset_y -= step / zoom;
        if (GetAsyncKeyState(VK_DOWN))  offset_y += step / zoom;

        txBegin();

        RGBQUAD* pixels = txVideoMemory();
        double world_x = 0;
        double world_y = 0;


        for (int iy = 0; iy < height; iy++) {
            world_y = offset_y + iy / zoom;

            for (int ix = 0; ix < width; ix++) {
                world_x = offset_x + ix / zoom;
                int flag = 0;
                double x = world_x, y = world_y;

                int ni = 0;
                for (ni = 0; ni < n_max; ni++) {
                    double x2 = x * x;
                    double y2 = y * y;
                    double xy = x * y;
                    double r2 = x2 + y2;

                    if (r2 >= r2_max) {
                        flag = 1;
                        break;
                    }
                    x = x2 - y2 + world_x;
                    y = xy + xy + world_y;
                }
                if (flag) {
                    int index = ix + (height - 1 - iy) * width;

                    int red = (int) (world_x*(ni + 1) + world_y/(ni + 1));
                    int green  = (int) (world_x*(ni + 1));
                    int blue = (int) (world_x*world_y*ni);

                    pixels[index].rgbRed   = (BYTE) ((ni + green * red + 69) % 68 + 81);
                    pixels[index].rgbGreen = (BYTE) (0);
                    pixels[index].rgbBlue  = (BYTE) (((red * ni + blue/(ni + 1) + 27) % 67) + 100);

                } else {
                    int index = ix + (height - 1 - iy) * width;
                    pixels[index].rgbRed   = 0;
                    pixels[index].rgbGreen = 0;
                    pixels[index].rgbBlue  = 0;
                }
            }
        }

        txEnd();
    }
}
