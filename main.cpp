// Includes 
#include "imaging/tgaimage.h"
#include "imaging/model.h"
#include "imaging/geometry.h"
#include <ctime>
#include <iostream>



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

const int width = 300;
const int height = 300;



// - - - | - - - | - - -
//Math functions





Vec3f barycentric(Vec2i* THREE_POINTS, Vec2i TARGET_POINT)
{
    Vec3f u = Vec3f(THREE_POINTS[2].x - THREE_POINTS[0].x, THREE_POINTS[1].x - THREE_POINTS[0].x, THREE_POINTS[0].x - TARGET_POINT.x) ^ Vec3f(THREE_POINTS[2].y - THREE_POINTS[0].y, THREE_POINTS[1].y - THREE_POINTS[0].y, THREE_POINTS[0].y - TARGET_POINT.y);
    if (std::abs(u.z) < 1)
    {
        return Vec3f(-1, 1, 1);
    }
    return Vec3f(1.f - (u.x + u.y)/u.z, u.y/u.z, u.x/u.z);
}

// - - - | - - - | - - -

// Drawing functions




void makeLine(Vec2i START, Vec2i END, TGAImage* IMAGE, const TGAColor COLOR)
{
    bool steep = false;
    // The major axis is the steeper one.
    if (std::abs(START.x - END.x) < std::abs(START.y - END.y))
    {
        std::swap(START.x, START.y);
        std::swap(END.x, END.y);
        steep = true;
    }
    //Drawing left to write, top to bottom
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

/*void drawTriangleFilled(Vec2i VERTEX_0, Vec2i VERTEX_1, Vec2i VERTEX_2, TGAImage* IMAGE, const TGAColor COLOR)
{
    bool steep = false;
    // The major axis is the steeper one.
    if (std::abs(VERTEX_0.x - VERTEX_1.x) < std::abs(VERTEX_0.y - VERTEX_1.y))
    {
        std::swap(VERTEX_0.x, VERTEX_0.y);
        std::swap(VERTEX_1.x, VERTEX_1.y);
        steep = true;
    }
    //Drawing left to write, top to bottom
    if (VERTEX_0.x > VERTEX_1.x)
    {
        std::swap(VERTEX_0, VERTEX_1);
    }
    int deltaX = VERTEX_1.x - VERTEX_0.x;
    int deltaY = VERTEX_1.y - VERTEX_0.y;
    float slope = std::abs(deltaY) * 2;
    float error = 0;
    int y = VERTEX_0.y;
    for (int x = VERTEX_0.x; x <= VERTEX_1.x; x++)
    {
        if (steep)
        {
            Vec2i point(y, x);
            makeLine(VERTEX_2, point, IMAGE, red);
            IMAGE->set(y, x, COLOR);
        }
        else
        {
            Vec2i point(x, y);
            makeLine(VERTEX_2, point, IMAGE, red);
            IMAGE->set(x, y, COLOR);
        }
        error += slope;
        if (error > deltaX)
        {
            y += VERTEX_1.y > VERTEX_0.y ? 1 : -1;
            error -= deltaX * 2;
        }
    }
}
*/
// - - -

void drawTriangleFilled(Vec2i* VERTEXES, TGAImage* IMAGE, const TGAColor COLOR)
{
    Vec2i boundingBoxMin(IMAGE->get_width() - 1, IMAGE->get_height() - 1);
    Vec2i boundingBoxMax(0, 0);
    Vec2i clamp = boundingBoxMin;

    for (int i = 0; i < 3; ++i)
    {
        boundingBoxMin.x = std::max(0, std::min(boundingBoxMin.x, VERTEXES[i].x));
        boundingBoxMin.y = std::max(0, std::min(boundingBoxMin.y, VERTEXES[i].y));
        boundingBoxMax.x = std::min(clamp.x, std::max(boundingBoxMax.x, VERTEXES[i].x));
        boundingBoxMax.y = std::min(clamp.y, std::max(boundingBoxMax.y, VERTEXES[i].y));
    }

    Vec2i pixel;
    for (pixel.x = boundingBoxMin.x; pixel.x <= boundingBoxMax.x; ++pixel.x)
    {
        for (pixel.y = boundingBoxMin.y; pixel.y <= boundingBoxMax.y; ++pixel.y)
        {
            Vec3f bc_screen = barycentric(VERTEXES, pixel);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
            {
                continue;
            }
            IMAGE->set(pixel.x, pixel.y, COLOR);
        }
    }
}

// - - -

void drawModel(Model* MODEL, TGAImage* IMAGE, Vec3f LIGHT_DIRECTION)
{
    for (int i = 0; i < MODEL->nfaces(); i++)
    {
        std::vector<int> face = MODEL->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];

        for (int j = 0; j < 3; j++)
        {
            Vec3f v = MODEL->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            world_coords[j] = v;
        }

        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * LIGHT_DIRECTION;

        if (intensity > 0)
        {
            drawTriangleFilled(screen_coords, IMAGE, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
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
        //draw triangle with a random color
        drawTriangle(Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2), IMAGE, cyan);
    }
}

// - - -

void fillTriangleDriver(TGAImage* IMAGE)
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
        Vec2i vertexes[3] = {Vec2i(x0, y0), Vec2i(x1, y1), Vec2i(x2, y2)};
        //draw triangle with a random color
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
    Vec3f lightDirection;
    std::cout << "Enter the light direction: ";
    std::cin >> lightDirection.x;
    std::cin >> lightDirection.y;
    std::cin >> lightDirection.z;
    drawModel(model, IMAGE, lightDirection);
}

// - - - | - - - | - - - 
// Driver



int main(int argc, char** argv)
{
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
    //write to the output directory with the file name as current time 
    std::time_t result = std::time(nullptr);
    std::string time = std::to_string(result);
    std::string outputFileName = "../output/" + time + ".tga";
    image.write_tga_file(const_cast<char*>(outputFileName.c_str()));
    return 0;
}
