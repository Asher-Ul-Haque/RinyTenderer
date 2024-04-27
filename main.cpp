            // Includes 
#include "imaging/tgaimage.h"
#include <iostream>

// - - - | - - - | - - -

            // Colors
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

// - - - | - - - | - - -

            // Drawing functions
void makeLine(int X_START, int Y_START, int X_END, int Y_END, float DELTA, TGAImage* IMAGE)
{
    bool steep = false;
    if (std::abs(X_START - X_END) < std::abs(Y_START - Y_END))
    {
        std::swap(X_START, Y_START);
        std::swap(X_END, Y_END);
        steep = true;
    }
    if (X_START > X_END)
    {
        std::swap(X_START, X_END);
        std::swap(Y_START, Y_END);
    }
    for (int x = X_START; x <= X_END; x++)
    {
        float point = (x - X_START) / (float)(X_END - X_START);
        int y = Y_START * (1.0f - point) + Y_END * point;
        if (steep)
        {
            IMAGE->set(y, x, red);
        }
        else
        {
            IMAGE->set(x, y, red);
        }
    }
}

// - - - | - - - | - - - 

            // Driver
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
