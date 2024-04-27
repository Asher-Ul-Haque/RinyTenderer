#include "imaging/tgaimage.h"
#include <iostream>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

void makeLine(int X_START, int Y_START, int X_END, int Y_END, float DELTA, TGAImage* IMAGE)
{
    int x, y;
    for (float point = 0; point < 1.0f; point += DELTA)
    {
        x = X_START + (X_END - X_START) * point;
        y = Y_START + (Y_END - Y_START) * point;
        IMAGE->set(x, y, white);
    }
}

int main(int argc, char** argv)
{
    TGAImage image(100, 100, TGAImage::RGB);
    int x0, x1, y0, y1;
    float delta;
    std::cout << "Enter x0, x1, y0, y1, delta: ";
    std::cin >> x0;
    std::cin >> x1;
    std::cin >> y0;
    std::cin >> y1;
    std::cin >> delta;
    makeLine(x0, y0, x1, y1, delta, &image);
    image.flip_vertically();
    image.write_tga_file("../../output/output.tga");
    return 0;
}
