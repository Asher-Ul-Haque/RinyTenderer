//INCLUDES
#include <vector>
#include <cmath>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"


// - - -


//LIGHTS CAMERA ACITON
const int width  = 800;
const int height = 800;
const int depth  = 255;

Model* model = NULL;
int* zBuffer = NULL;
Vec3f lightDirection(0,1,-1);
Vec3f camera(0,0,5);


// - - - | - - - | - - -


//Math functions
Vec3f convertMatrixToVector(Matrix MATRIX) 
{
    return Vec3f(MATRIX[0][0] / MATRIX[3][0], MATRIX[1][0] / MATRIX[3][0], MATRIX[2][0] / MATRIX[3][0]);
}


// - - -


Matrix convertVectorToMatrix(Vec3f VECTOR) 
{
    Matrix matrix(4, 1);
    matrix[0][0] = VECTOR.x;
    matrix[1][0] = VECTOR.y;
    matrix[2][0] = VECTOR.z;
    matrix[3][0] = 1.f;
    return matrix;
}


// - - -


Matrix viewport(int X, int Y, int WIDTH, int HEIGHT) 
{
    Matrix matrix = Matrix::identity(4);
    matrix[0][3] = X + WIDTH / 2.0f;
    matrix[1][3] = Y + HEIGHT / 2.0f;
    matrix[2][3] = depth / 2.0f;

    matrix[0][0] = WIDTH / 2.0f;
    matrix[1][1] = HEIGHT / 2.0f;
    matrix[2][2] = depth / 2.0f;
    return matrix;
}


// - - - | - - -


//Drawing functions
void triangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec2i uv0, Vec2i uv1, Vec2i uv2, TGAImage &image, float intensity, int *zBuffer) {
    if (t0.y==t1.y && t0.y==t2.y) return; // i dont care about degenerate triangles
    if (t0.y>t1.y) { std::swap(t0, t1); std::swap(uv0, uv1); }
    if (t0.y>t2.y) { std::swap(t0, t2); std::swap(uv0, uv2); }
    if (t1.y>t2.y) { std::swap(t1, t2); std::swap(uv1, uv2); }

    int total_height = t2.y-t0.y;
    for (int i=0; i<total_height; i++) {
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; // be careful: with above conditions no division by zero here
        Vec3i A   =               t0  + Vec3f(t2-t0  )*alpha;
        Vec3i B   = second_half ? t1  + Vec3f(t2-t1  )*beta : t0  + Vec3f(t1-t0  )*beta;
        Vec2i uvA =               uv0 +      (uv2-uv0)*alpha;
        Vec2i uvB = second_half ? uv1 +      (uv2-uv1)*beta : uv0 +      (uv1-uv0)*beta;
        if (A.x>B.x) { std::swap(A, B); std::swap(uvA, uvB); }
        for (int j=A.x; j<=B.x; j++) {
            float phi = B.x==A.x ? 1. : (float)(j-A.x)/(float)(B.x-A.x);
            Vec3i   P = Vec3f(A) + Vec3f(B-A)*phi;
            Vec2i uvP =     uvA +   (uvB-uvA)*phi;
            int idx = P.x+P.y*width;
            if (zBuffer[idx]<P.z) {
                zBuffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                image.set(P.x, P.y, TGAColor(color.r*intensity, color.g*intensity, color.b*intensity));
            }
        }
    }
}


// - - - | - - - | - - -


//Driver
int main(int argc, char** argv) {
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }

    zBuffer = new int[width*height];
    for (int i=0; i<width*height; i++) {
        zBuffer[i] = std::numeric_limits<int>::min();
    }

    { // draw the model
        Matrix Projection = Matrix::identity(4);
        Matrix ViewPort   = viewport(width/8, height/8, width*3/4, height*3/4);
        Projection[3][2] = -1.f/camera.z;

        TGAImage image(width, height, TGAImage::RGB);
        for (int i=0; i<model->nfaces(); i++) {
            std::vector<int> face = model->face(i);
            Vec3i screen_coords[3];
            Vec3f world_coords[3];
            for (int j=0; j<3; j++) {
                Vec3f v = model->vert(face[j]);
                screen_coords[j] =  convertMatrixToVector(ViewPort*Projection*convertVectorToMatrix(v));
                world_coords[j]  = v;
            }
            Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
            n.normalize();
            float intensity = n*lightDirection;
            if (intensity>0) 
            {
                Vec2i uv[3];
                for (int k=0; k<3; k++) 
                {
                    uv[k] = model->uv(i, k);
                }
                triangle(screen_coords[0], screen_coords[1], screen_coords[2], uv[0], uv[1], uv[2], image, intensity, zBuffer);
            }
        }

        image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        image.write_tga_file("output/image.tga");
    }

    { // dump z-buffer (debugging purposes only)
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i=0; i<width; i++) 
        {
            for (int j=0; j<height; j++) 
            {
                zbimage.set(i, j, TGAColor(zBuffer[i+j*width], 1));
            }
        }
        zbimage.flip_vertically(); // i want to have the origin at the left bottom corner of the image
        zbimage.write_tga_file("output/zbuffer.tga");
    }
    delete model;
    delete [] zBuffer;
    return 0;
}

