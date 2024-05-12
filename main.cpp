// Includes 
#include "imaging/tgaimage.h"
#include "imaging/model.h"
#include "imaging/geometry.h"
#include <ctime>
#include <iostream>
#include <limits>


// - - - | - - - | - - -


// Colors and Models
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const TGAColor yellow = TGAColor(255, 255, 0, 255);
const TGAColor cyan = TGAColor(0, 255, 255, 255);
const TGAColor magenta = TGAColor(255, 0, 255, 255);
const TGAColor black = TGAColor(0, 0, 0, 255);

Model* model = NULL;


// - - -


//Sizes and buffers
const int width = 1000;
const int height = 800;

float zBuffer[width * height];


// - - - | - - - | - - -
//Math functions


Vec3f getBarycentricCoordinates(Vec3f* TRIANGLE_VERTICES, Vec3f TARGET_POINT)
{
    Vec3f edgeVectors[2];
    for (int i  = 0; i < 2; ++i)
    {
        edgeVectors[i].x = TRIANGLE_VERTICES[2][i] - TRIANGLE_VERTICES[0][i];
        edgeVectors[i].y = TRIANGLE_VERTICES[1][i] - TRIANGLE_VERTICES[0][i];
        edgeVectors[i].z = TRIANGLE_VERTICES[0][i] - TARGET_POINT[i];
    }
    Vec3f u = cross(edgeVectors[0], edgeVectors[1]);
    if (std::abs(u[2]) > 0.01f)
    {
        return Vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    }
    return Vec3f(-1, 1, 1);
}


// - - -


Vec3f worldToScreenCoordinates(Vec3f WORLD_COORDINATES)
{
    return Vec3f(int((WORLD_COORDINATES.x + 1.0f) * width / 2.0f + 0.5f), int((WORLD_COORDINATES.y + 1.0f) * height / 2.0f + 0.5f), WORLD_COORDINATES.z);
}


// - - - | - - - | - - -
// Drawing functions


void makeLine(Vec2i START, Vec2i END, TGAImage* IMAGE, const TGAColor COLOR)
{
    bool steep = false;
    if (std::abs(START.x - END.x) < std::abs(START.y - END.y))
    {
        std::swap(START.x, START.y);
        std::swap(END.x, END.y);
        steep = true;
    }
    if (START.x > END.x)
    {
        std::swap(START, END);
    }
    int deltaX = END.x - START.x;
    int deltaY = END.y - START.y;
    float slope = std::abs(deltaY) * 2;
    float error = 0;
    int y = START.y;
    for (int x = START.x; x <= END.x; x++)
    {
        if (steep)
        {
            IMAGE->set(y, x, COLOR);
        }
        else
        {
            IMAGE->set(x, y, COLOR);
        }
        error += slope;
        if (error > deltaX)
        {
            y += END.y > START.y ? 1 : -1;
            error -= deltaX * 2;
        }
    }

}


// - - - 


void drawTriangle(Vec2i VERTEX_0, Vec2i VERTEX_1, Vec2i VERTEX_3, TGAImage* IMAGE, const TGAColor COLOR)
{
    makeLine(VERTEX_0, VERTEX_1, IMAGE, COLOR);
    makeLine(VERTEX_1, VERTEX_3, IMAGE, COLOR);
    makeLine(VERTEX_3, VERTEX_0, IMAGE, COLOR);
}

// - - -


void drawTriangleFilled(Vec3f* VERTEXES, TGAImage* IMAGE, const TGAColor COLOR)
{
    Vec2f boundingBoxMin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f boundingBoxMax(std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(IMAGE->get_width() - 1, IMAGE->get_height() - 1);

    for (int i = 0; i < 3; ++i)
    {
        boundingBoxMin.x = std::max(0.0f, std::min(boundingBoxMin.x, VERTEXES[i].x));
        boundingBoxMin.y = std::max(0.0f, std::min(boundingBoxMin.y, VERTEXES[i].y));
        boundingBoxMax.x = std::min(clamp.x, std::max(boundingBoxMax.x, VERTEXES[i].x));
        boundingBoxMax.y = std::min(clamp.y, std::max(boundingBoxMax.y, VERTEXES[i].y));
    }

    Vec3f pixel;
    for (pixel.x = boundingBoxMin.x; pixel.x <= boundingBoxMax.x; ++pixel.x)
    {
        for (pixel.y = boundingBoxMin.y; pixel.y <= boundingBoxMax.y; ++pixel.y)
        {
            Vec3f screenCoordinates = getBarycentricCoordinates(VERTEXES, pixel);
            if (screenCoordinates.x < 0 || screenCoordinates.y < 0 || screenCoordinates.z < 0)
            {
                continue;
            }
            pixel.z = 0;
            for (int i = 0; i < 3; ++i)
            {
                pixel.z += VERTEXES[i].z * screenCoordinates[i];
            }
            if (zBuffer[int(pixel.x + pixel.y * width)] < pixel.z)
            {
                zBuffer[int(pixel.x + pixel.y * width)] = pixel.z;
                IMAGE->set(pixel.x, pixel.y, COLOR);
            }
        }
    }
}


// - - -


void  drawModel(TGAImage* IMAGE)
{
    for (int i = 0; i < model->nfaces(); ++i)
    {
        std::vector<int> face = model->face(i);
        Vec3f points[3];
        for (int i = 0; i < 3; ++i)
        {
            points[i] = worldToScreenCoordinates(model->vert(face[i]));
        }
        drawTriangleFilled(points, IMAGE, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }
}


// - - - | - - -
// Driver functions


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
        Vec2i start(x0, y0);
        Vec2i end(x1, y1);
        makeLine(start, end, IMAGE, white);
    }
}


// - - -


void triangleDriver(TGAImage* IMAGE)
{
    int count;
    std::cout << "Enter the number of triangles: ";
    std::cin >> count;
    int x0, x1, x2, y0, y1, y2;
    for (int i = 0; i < count; i++)
    {
        std::cout << "Enter x0, y0, x1, y1, x2, y2 : ";
        std::cin >> x0;
        std::cin >> y0;
        std::cin >> x1;
        std::cin >> y1;
        std::cin >> x2;
        std::cin >> y2;
        drawTriangle(Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2), IMAGE, cyan);
    }
}


// - - -


void fillTriangleDriver(TGAImage* IMAGE)
{
    int count;
    std::cout << "Enter the number of triangles: ";
    std::cin >> count;
    int x0, x1, x2, y0, y1, y2, z0, z1, z2;
    for (int i = 0; i < count; i++)
    {
        std::cout << "Enter x0, y0, z0, x1, y1, z1, x2, y2, z2 : ";
        std::cin >> x0;
        std::cin >> y0;
        std::cin >> z0;
        std::cin >> x1;
        std::cin >> y1;
        std::cin >> z1;
        std::cin >> x2;
        std::cin >> y2;
        std::cin >> z2;
        Vec3f vertexes[3] = {Vec3f(x0, y0, z0), Vec3f(x1, y1, z1), Vec3f(x2, y2, z2)};
        drawTriangleFilled(vertexes, IMAGE, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
    }
}


// - - - 


void modelDriver(TGAImage* IMAGE)
{
    std::string path;
    std::cout << "Enter the path to the model: ";
    std::cin >> path;
    model = new Model(path.c_str());
    drawModel(IMAGE);
}


// - - - | - - - | - - - 
// Driver


int main(int argc, char** argv)
{
    for (int i = 0; i < width * height; ++i)
    {
        zBuffer[i] = -std::numeric_limits<float>::max();
    }
    TGAImage image(width, height, TGAImage::RGB);
    int choice;
    std::cout << "1. Draw lines\n2. Draw triangles\n3. Fill triangles\n4. Draw model\nEnter your choice: ";
    std::cin >> choice;
    switch (choice)
    {
        case 1:
            lineDriver(&image);
            break;

        case 2:
            triangleDriver(&image);
            break;

        case 3:
            fillTriangleDriver(&image);
            break;

        case 4:
            modelDriver(&image);
            break;

        default:
            std::cout << "Invalid choice\n";
            break;
    }
    image.flip_vertically();
    std::time_t result = std::time(nullptr);
    std::string time = std::to_string(result);
    std::string outputFileName = "../output/" + time + ".tga";
    const char* output = outputFileName.c_str();
    image.write_tga_file(output);
    std::string command = "xdg-open " + outputFileName;
    system(command.c_str());
    return 0;
}
