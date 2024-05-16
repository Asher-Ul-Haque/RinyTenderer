#include <vector>
#include <cmath>
#include <limits>
#include <iostream>
#include "imaging/tgaimage.h"
#include "imaging/model.h"
#include "imaging/geometry.h"

const int width  = 800;
const int height = 800;
const int depth  = 255;

Model *model = NULL;
int *zbuffer = NULL;
Vec3f light_dir(0,1,-1);
Vec3f camera(0,0,5);

Vec3f convertMatrixToVector(Matrix MATRIX) 
{
    return Vec3f(MATRIX[0][0] / MATRIX[3][0], MATRIX[1][0] / MATRIX[3][0], MATRIX[2][0] / MATRIX[3][0]);
}

Matrix convertVectorToMatrix(Vec3f VECTOR) 
{
    Matrix matrix(4, 1);
    matrix[0][0] = VECTOR.x;
    matrix[1][0] = VECTOR.y;
    matrix[2][0] = VECTOR.z;
    matrix[3][0] = 1.f;
    return matrix;
}

Matrix viewport(int X, int Y, int WIDTH, int HEIGHT) 
{
    Matrix matrix = Matrix::identity(4);
    matrix[0][3] = X + WIDTH / 2.0f;
    matrix[1][3] = Y + HEIGHT / 2.0f;
    matrix[2][3] = depth/2.f;

    matrix[0][0] = WIDTH / 2.0f;
    matrix[1][1] = HEIGHT / 2.0f;
    matrix[2][2] = depth/2.f;

    return matrix;
}

void drawTriangle(Vec3i VERTEX_0, Vec3i VERTEX_1, Vec3i VERTEX_2, Vec2i UV_0, Vec2i UV_1, Vec2i UV_2, TGAImage* IMAGE, float LIGHT_SRC_INTENSITY, int *Z_BUFFER) 
{
    if (VERTEX_0.y == VERTEX_1.y && VERTEX_0.y == VERTEX_2.y) return;

    if (VERTEX_0.y > VERTEX_1.y) 
    { 
        std::swap(VERTEX_0, VERTEX_1); 
        std::swap(UV_0, UV_1); 
    }
    if (VERTEX_0.y > VERTEX_2.y) 
    { 
        std::swap(VERTEX_0, VERTEX_2); 
        std::swap(UV_0, UV_2); 
    }
    if (VERTEX_1.y>VERTEX_2.y) 
    { 
        std::swap(VERTEX_1, VERTEX_2); 
        std::swap(UV_1, UV_2); 
    }

    int totalHeight = VERTEX_2.y-VERTEX_0.y;
    for (int i = 0; i < totalHeight; ++i) 
    {
        bool secondHalf = i > VERTEX_1.y - VERTEX_0.y || VERTEX_1.y == VERTEX_0.y;
        int segmentHeight = secondHalf ? VERTEX_2.y - VERTEX_1.y : VERTEX_1.y - VERTEX_0.y;
        float alpha = (float) i / totalHeight;
        float beta  = (float) (i - (secondHalf ? VERTEX_1.y - VERTEX_0.y : 0)) / segmentHeight; // be careful: with above conditions no division by zero here
        Vec3i A = VERTEX_0  + Vec3f(VERTEX_2-VERTEX_0  ) * alpha;
        Vec3i B = secondHalf ? VERTEX_1 + Vec3f(VERTEX_2 - VERTEX_1  ) * beta : VERTEX_0  + Vec3f(VERTEX_1 - VERTEX_0) * beta;
        Vec2i uvA = UV_0 + (UV_2-UV_0) * alpha;
        Vec2i uvB = secondHalf ? UV_1 + (UV_2 - UV_1) * beta : UV_0 + (UV_1 - UV_0) * beta;
        if (A.x > B.x) 
        { 
            std::swap(A, B); 
            std::swap(uvA, uvB); 
        }
        for (int j= A.x; j <= B.x; ++j) 
        {
            float phi = B.x == A.x ? 1.0f : (float) (j - A.x) / (float) (B.x - A.x);
            Vec3i P = Vec3f(A) + Vec3f(B - A) * phi;
            Vec2i uvP = uvA + (uvB - uvA) * phi;
            int idx = P.x + P.y * width;
            if (Z_BUFFER[idx] < P.z) 
            {
                Z_BUFFER[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                IMAGE->set(P.x, P.y, TGAColor(color.r * LIGHT_SRC_INTENSITY, color.g * LIGHT_SRC_INTENSITY, color.b * LIGHT_SRC_INTENSITY));
            }
        }
    }
}

int main(int argc, char** argv) 
{
    std::cout << "Enter path to 3d model:  ";
    std::string path;
    std::cin >> path;

    Model* model = new Model("african_head.obj");

    zbuffer = new int[width * height];
    for (int i = 0; i < width * height; ++i) 
    {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    { // draw the model
        Matrix Projection = Matrix::identity(4);
        Matrix ViewPort   = viewport(width / 8, height / 8, width * 3/4, height * 3/4);
        Projection[3][2] = -1.0f / camera.z;

        TGAImage image(width, height, TGAImage::RGB);
        for (int i = 0; i < model->nfaces(); ++i) 
        {
            std::vector<int> face = model->face(i);
            Vec3i screenCoords[3];
            Vec3f worldCoords[3];
            for (int j = 0; j < 3; ++j) 
            {
                Vec3f v = model->vert(face[j]);
                screenCoords[j] =  convertMatrixToVector(ViewPort * Projection * convertVectorToMatrix(v));
                worldCoords[j] = v;
            }
            Vec3f n = (worldCoords[2] - worldCoords[0]) ^ (worldCoords[1] - worldCoords[0]);
            n.normalize();
            float intensity = n * light_dir;
            if (intensity>0) 
            {
                Vec2i uv[3];
                for (int k = 0; k < 3; ++k) 
                {
                    uv[k] = model->uv(i, k);
                }
                drawTriangle(screenCoords[0], screenCoords[1], screenCoords[2], uv[0], uv[1], uv[2], &image, intensity, zbuffer);
            }
        }

        image.flip_vertically();
        image.write_tga_file("output.tga");
    }

    {
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i=0; i<width; i++) 
        {
            for (int j=0; j<height; j++) 
            {
                zbimage.set(i, j, TGAColor(zbuffer[i+j*width], 1));
            }
        }
        zbimage.flip_vertically();
        zbimage.write_tga_file("zbuffer.tga");
    }
    delete model;
    delete [] zbuffer;
    return 0;
}
