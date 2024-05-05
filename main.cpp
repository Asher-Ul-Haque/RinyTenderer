// Includes 
#include "imaging/tgaimage.h"
#include <iostream>
#include "imaging/model.h"
#include "imaging/geometry.h"

// - - - | - - - | - - -

// Colors and Models
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
Model* model = NULL;
const int width = 800;
const int height = 800;

// - - - | - - - | - - -

// Drawing functions
void makeLine(int X_START, int Y_START, int X_END, int Y_END, TGAImage* IMAGE)
{
    bool steep = false;
    // The major axis is the steeper one.
    if (std::abs(X_START - X_END) < std::abs(Y_START - Y_END))
    {
        std::swap(X_START, Y_START);
        std::swap(X_END, Y_END);
        steep = true;
    }
    //Drawing left to write, top to bottom
    if (X_START > X_END)
    {
        std::swap(X_END, X_START);
        std::swap(Y_END, Y_START);
    }
    int deltaX = X_END - X_START;
    int deltaY = Y_END - Y_START;
    float slope = std::abs(deltaY) * 2;
    float error = 0;
    int y = Y_START;
    for (int x = X_START; x <= X_END; x++)
    {
        if (steep)
        {
            IMAGE->set(y, x, white);
        }
        else
        {
            IMAGE->set(x, y, white);
        }
        error += slope;
        if (error > deltaX)
        {
            y += Y_END > Y_START ? 1 : -1;
            error -= deltaX * 2;
        }
    }
}

// - - -

void lineDriver(TGAImage* IMAGE)
{
    int count;
    std::cout << "Enter the number of lines: ";
    std::cin >> count;
    int x0, x1, y0, y1;
    for (int i = 0; i < count; i++)
    {
        std::cout << "Enter x0, y0, x1, y1 : ";
        std::cin >> x0;
        std::cin >> y0;
        std::cin >> x1;
        std::cin >> y1;
        makeLine(x0, y0, x1, y1, IMAGE);
    }
}

// - - - | - - - | - - - 

// Driver
int main(int argc, char** argv)
{
    switch (argc)
    {
        case 2:
            model = new Model(argv[1]);
            break;

        default:
            model = new Model("../../models/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);
    for (int i=0; i<model->nfaces(); i++) 
    {
        std::vector<int> face = model->face(i);
        for (int j=0; j<3; j++) 
        {
            Vec3f v0 = model->vert(face[j]);
            Vec3f v1 = model->vert(face[(j+1)%3]);
            int x0 = (v0.x+1.)*width/2.;
            int y0 = (v0.y+1.)*height/2.;
            int x1 = (v1.x+1.)*width/2.;
            int y1 = (v1.y+1.)*height/2.;
            makeLine(x0, y0, x1, y1, &image);
        }
    }

    image.flip_vertically();
    image.write_tga_file("../../output/headWireframe.tga");
    delete model;
    return 0;
}
