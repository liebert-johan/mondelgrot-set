
#include <TXLib.h>

#include <stdio.h>



int main(){
    int width = 800;
    int height = 600;
    float zoom = 200.0f;
    float offset_x = -2.5f;
    float offset_y = 1.25f;
    float step = 20;

    int n_max = 256;
    float r2_max = 4.0f;

    txCreateWindow(800, 600);
    txClearConsole();

    while (!GetAsyncKeyState(VK_ESCAPE)) {

        if (GetAsyncKeyState(VK_OEM_PLUS)) {
        float old_zoom = zoom;
        zoom *= 1.05f;
        offset_x += ((float)width  / 2.0f) / old_zoom - ((float)width  / 2.0f) / zoom;
        offset_y += ((float)height / 2.0f) / old_zoom - ((float)height / 2.0f) / zoom;
    }

    if (GetAsyncKeyState(VK_OEM_MINUS)) {
        float old_zoom = zoom;
        zoom /= 1.05f;
        offset_x += ((float)width  / 2.0f) / old_zoom - ((float)width  / 2.0f) / zoom;
        offset_y += ((float)height / 2.0f) / old_zoom - ((float)height / 2.0f) / zoom;
    }
        if (GetAsyncKeyState(VK_LEFT))  offset_x -= step / zoom;
        if (GetAsyncKeyState(VK_RIGHT)) offset_x += step / zoom;
        if (GetAsyncKeyState(VK_UP))    offset_y -= step / zoom;
        if (GetAsyncKeyState(VK_DOWN))  offset_y += step / zoom;

        txBegin();

        RGBQUAD* pixels = txVideoMemory();
        float world_x = 0;
        float world_y = 0;


        for (int iy = 0; iy < height; iy += 1) {
            world_y = offset_y + (float)iy / zoom;
            float y0[4] = {world_y, world_y, world_y, world_y};


            for (int ix = 0; ix < width; ix += 4) {
                world_x = offset_x + (float)ix / zoom;
                float pix = 1.0f/zoom;
                float x0[4]= {world_x, world_x + pix, world_x + 2*pix, world_x + 3*pix};


                float x[4] = {}; for (int i = 0; i < 4; i++) x[i] = x0[i];
                float y[4] = {}; for (int i = 0; i < 4; i++) y[i] = y0[i];

                int N[4] = {0, 0, 0, 0};
                int ni = 0;

                for (ni = 0; ni < n_max; ni++) {
                    float x2[4] = {};
                    for (int i = 0; i < 4; i++) x2[i] = x[i] * x[i];

                    float y2[4] = {};  // объявление массива из 4 элементов, все = 0
                    for (int i = 0; i < 4; i++) y2[i] = y[i] * y[i];

                    float xy[4] = {};
                    for (int i = 0; i < 4; i++) xy[i] = x[i] * y[i];

                    float r2[4] = {};
                    for (int i = 0; i < 4; i++) r2[i] = x2[i] + y2[i];

                    int cmp[4] = {0, 0, 0, 0};

                    for (int i = 0; i < 4; i++) if (r2[i] <= r2_max && r2[i] == r2[i]) cmp[i] = 1;


                    for (int i = 0; i < 4; i++) {
                        if (cmp[i]) {
                            x[i] = x2[i] - y2[i] + x0[i];
                            y[i] = xy[i] + xy[i] + y0[i];
                        }

                    }
                    int mask = 0;
                    for (int i = 0; i < 4; i++) mask |= (cmp[i] << i);
                    if (!mask) break;

                    for (int i = 0; i < 4; i++) N[i] += cmp[i];
                }

                for (int i = 0; i < 4; i++) {
                    int index = ix + i + (height - 1 - iy) * width;
                    if (N[i] < 256) {
                        int red = (int) (world_x*((float)ni + 1) + world_y/((float)ni + 1));
                        int green  = (int) (world_x*((float)ni + 1));
                        int blue = (int) (world_x*world_y* ((float)ni));

                        pixels[index].rgbRed   = (BYTE) ((ni + green * red + 69) % 68 + 81);
                        pixels[index].rgbGreen = (BYTE) (0);
                        pixels[index].rgbBlue  = (BYTE) (((red * ni + blue/(ni + 1) + 27) % 67) + 100);
                    } else {
                        pixels[index].rgbRed   = 0;
                        pixels[index].rgbGreen = 0;
                        pixels[index].rgbBlue  = 0;
                    }

                }
            }
        }
        txEnd();
    }
}
