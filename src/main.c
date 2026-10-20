#include "rinyTenderer/renderer/light.h"
#include <justLibrary.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/renderer/mesh.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

// 1. Cube Mesh (Flat Shaded - 24 Vertices, 6 Uniform Colored Faces)
static TerminalMesh createCubeMesh(void)
{
    terminalUpdateSize();
    justMemorySetLimit(1024 * 1024, "TERM_BUFFER");
    justMemorySetLimit(1024 * 1024, "TERM_MESH");
    TerminalMesh cube;
    meshCreate(&cube, 24, 12);

    // Front Face (Z = 1.0f) - Red
    cube.vertices[0]  = (TerminalVertex){ .position = {{-1.0f, -1.0f,  1.0f}}, .normal = {{0.0f, 0.0f, 1.0f}}, .color = (RGBcolor){255, 50,  50} };
    cube.vertices[1]  = (TerminalVertex){ .position = {{ 1.0f, -1.0f,  1.0f}}, .normal = {{0.0f, 0.0f, 1.0f}}, .color = (RGBcolor){255, 50,  50} };
    cube.vertices[2]  = (TerminalVertex){ .position = {{ 1.0f,  1.0f,  1.0f}}, .normal = {{0.0f, 0.0f, 1.0f}}, .color = (RGBcolor){255, 50,  50} };
    cube.vertices[3]  = (TerminalVertex){ .position = {{-1.0f,  1.0f,  1.0f}}, .normal = {{0.0f, 0.0f, 1.0f}}, .color = (RGBcolor){255, 50,  50} };

    // Back Face (Z = -1.0f) - Green
    cube.vertices[4]  = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f}}, .normal = {{0.0f, 0.0f, -1.0f}}, .color = (RGBcolor){50,  255, 50} };
    cube.vertices[5]  = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f}}, .normal = {{0.0f, 0.0f, -1.0f}}, .color = (RGBcolor){50,  255, 50} };
    cube.vertices[6]  = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f}}, .normal = {{0.0f, 0.0f, -1.0f}}, .color = (RGBcolor){50,  255, 50} };
    cube.vertices[7]  = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f}}, .normal = {{0.0f, 0.0f, -1.0f}}, .color = (RGBcolor){50,  255, 50} };

    // Left Face (X = -1.0f) - Blue
    cube.vertices[8]  = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f}}, .normal = {{-1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){50,  50,  255} };
    cube.vertices[9]  = (TerminalVertex){ .position = {{-1.0f, -1.0f,  1.0f}}, .normal = {{-1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){50,  50,  255} };
    cube.vertices[10] = (TerminalVertex){ .position = {{-1.0f,  1.0f,  1.0f}}, .normal = {{-1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){50,  50,  255} };
    cube.vertices[11] = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f}}, .normal = {{-1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){50,  50,  255} };

    // Right Face (X = 1.0f) - Yellow
    cube.vertices[12] = (TerminalVertex){ .position = {{ 1.0f, -1.0f,  1.0f}}, .normal = {{1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){255, 255, 50} };
    cube.vertices[13] = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f}}, .normal = {{1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){255, 255, 50} };
    cube.vertices[14] = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f}}, .normal = {{1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){255, 255, 50} };
    cube.vertices[15] = (TerminalVertex){ .position = {{ 1.0f,  1.0f,  1.0f}}, .normal = {{1.0f, 0.0f, 0.0f}}, .color = (RGBcolor){255, 255, 50} };

    // Top Face (Y = 1.0f) - Magenta
    cube.vertices[16] = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f}}, .normal = {{0.0f, 1.0f, 0.0f}}, .color = (RGBcolor){255, 50,  255} };
    cube.vertices[17] = (TerminalVertex){ .position = {{-1.0f,  1.0f,  1.0f}}, .normal = {{0.0f, 1.0f, 0.0f}}, .color = (RGBcolor){255, 50,  255} };
    cube.vertices[18] = (TerminalVertex){ .position = {{ 1.0f,  1.0f,  1.0f}}, .normal = {{0.0f, 1.0f, 0.0f}}, .color = (RGBcolor){255, 50,  255} };
    cube.vertices[19] = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f}}, .normal = {{0.0f, 1.0f, 0.0f}}, .color = (RGBcolor){255, 50,  255} };

    // Bottom Face (Y = -1.0f) - Cyan
    cube.vertices[20] = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f}}, .normal = {{0.0f, -1.0f, 0.0f}}, .color = (RGBcolor){50,  255, 255} };
    cube.vertices[21] = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f}}, .normal = {{0.0f, -1.0f, 0.0f}}, .color = (RGBcolor){50,  255, 255} };
    cube.vertices[22] = (TerminalVertex){ .position = {{ 1.0f, -1.0f,  1.0f}}, .normal = {{0.0f, -1.0f, 0.0f}}, .color = (RGBcolor){50,  255, 255} };
    cube.vertices[23] = (TerminalVertex){ .position = {{-1.0f, -1.0f,  1.0f}}, .normal = {{0.0f, -1.0f, 0.0f}}, .color = (RGBcolor){50,  255, 255} };

    TerminalTriangle tempTriangles[12] = {
        { .indices = { 0,  1,  2  } }, { .indices = { 0,  2,  3  } }, // Front
        { .indices = { 4,  5,  6  } }, { .indices = { 4,  6,  7  } }, // Back
        { .indices = { 8,  9,  10 } }, { .indices = { 8,  10, 11 } }, // Left
        { .indices = { 12, 13, 14 } }, { .indices = { 12, 14, 15 } }, // Right
        { .indices = { 16, 17, 18 } }, { .indices = { 16, 18, 19 } }, // Top
        { .indices = { 20, 21, 22 } }, { .indices = { 20, 22, 23 } }  // Bottom
    };

    for (size_t i = 0; i < 12; ++i)
    {
        cube.triangles[i] = tempTriangles[i];
    }

    cube.transform.position = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    cube.transform.rotation = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    cube.transform.scale    = (vec3s){{ 1.8f, 1.8f, 1.8f }};

    return cube;
}

// 2. Triangular Pyramid Mesh (12 Vertices, 4 Uniform Colored Faces with Normals)
static TerminalMesh createPyramidMesh(void)
{
    TerminalMesh pyramid;
    meshCreate(&pyramid, 4, 4);

    pyramid.vertices[0] = (TerminalVertex){ .position = {{  0.0f,  1.0f,  0.0f }}, .color = (RGBcolor){ 255, 100, 100 } }; // Top Apex
    pyramid.vertices[1] = (TerminalVertex){ .position = {{ -1.0f, -1.0f,  1.0f }}, .color = (RGBcolor){ 100, 255, 100 } }; // Front-Left
    pyramid.vertices[2] = (TerminalVertex){ .position = {{  1.0f, -1.0f,  1.0f }}, .color = (RGBcolor){ 100, 100, 255 } }; // Front-Right
    pyramid.vertices[3] = (TerminalVertex){ .position = {{  0.0f, -1.0f, -1.0f }}, .color = (RGBcolor){ 255, 255, 100 } }; // Back

    TerminalTriangle tempTriangles[4] = {
        { .indices = { 0, 1, 2 } }, 
        { .indices = { 0, 2, 3 } }, 
        { .indices = { 0, 3, 1 } }, 
        { .indices = { 3, 2, 1 } }  
    };

    for (size_t i = 0; i < 4; ++i)
    {
        pyramid.triangles[i] = tempTriangles[i];
    }

    pyramid.transform.position = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    pyramid.transform.rotation = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    pyramid.transform.scale    = (vec3s){{ 1.8f, 1.8f, 1.8f }}; // Made bigger

    return pyramid;
}

// 3. Triangular Prism Mesh (18 Vertices, 5 Uniform Colored Faces with Normals)
static TerminalMesh createPrismMesh(void)
{
    TerminalMesh prism;
    meshCreate(&prism, 18, 8);

    // Side Face 1 (Quad -> 2 Triangles) - Magenta
    prism.vertices[0] = (TerminalVertex){ .position = {{ 0.0f, -1.0f,  1.0f }}, .normal = {{ 0.0f, 0.0f,  1.0f }}, .color = (RGBcolor){ 255, 0, 255 } };
    prism.vertices[1] = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f }}, .normal = {{ 0.0f, 0.0f,  1.0f }}, .color = (RGBcolor){ 255, 0, 255 } };
    prism.vertices[2] = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f }}, .normal = {{ 0.0f, 0.0f,  1.0f }}, .color = (RGBcolor){ 255, 0, 255 } };
    prism.vertices[3] = (TerminalVertex){ .position = {{ 0.0f,  1.0f,  1.0f }}, .normal = {{ 0.0f, 0.0f,  1.0f }}, .color = (RGBcolor){ 255, 0, 255 } };

    // Side Face 2 (Quad -> 2 Triangles) - Cyan
    prism.vertices[4] = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f }}, .normal = {{-0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 0, 255, 255 } };
    prism.vertices[5] = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f }}, .normal = {{-0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 0, 255, 255 } };
    prism.vertices[6] = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f }}, .normal = {{-0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 0, 255, 255 } };
    prism.vertices[7] = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f }}, .normal = {{-0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 0, 255, 255 } };

    // Side Face 3 (Quad -> 2 Triangles) - Yellow
    prism.vertices[8]  = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f }}, .normal = {{ 0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 255, 255, 0 } };
    prism.vertices[9]  = (TerminalVertex){ .position = {{ 0.0f, -1.0f,  1.0f }}, .normal = {{ 0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 255, 255, 0 } };
    prism.vertices[10] = (TerminalVertex){ .position = {{ 0.0f,  1.0f,  1.0f }}, .normal = {{ 0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 255, 255, 0 } };
    prism.vertices[11] = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f }}, .normal = {{ 0.866f, 0.0f, -0.5f }}, .color = (RGBcolor){ 255, 255, 0 } };

    // Bottom Cap - Orange
    prism.vertices[12] = (TerminalVertex){ .position = {{ 1.0f, -1.0f, -1.0f }}, .normal = {{ 0.0f, -1.0f, 0.0f }}, .color = (RGBcolor){ 255, 128, 0 } };
    prism.vertices[13] = (TerminalVertex){ .position = {{-1.0f, -1.0f, -1.0f }}, .normal = {{ 0.0f, -1.0f, 0.0f }}, .color = (RGBcolor){ 255, 128, 0 } };
    prism.vertices[14] = (TerminalVertex){ .position = {{ 0.0f, -1.0f,  1.0f }}, .normal = {{ 0.0f, -1.0f, 0.0f }}, .color = (RGBcolor){ 255, 128, 0 } };

    // Top Cap - Lime Green
    prism.vertices[15] = (TerminalVertex){ .position = {{ 0.0f,  1.0f,  1.0f }}, .normal = {{ 0.0f,  1.0f, 0.0f }}, .color = (RGBcolor){ 128, 255, 0 } };
    prism.vertices[16] = (TerminalVertex){ .position = {{-1.0f,  1.0f, -1.0f }}, .normal = {{ 0.0f,  1.0f, 0.0f }}, .color = (RGBcolor){ 128, 255, 0 } };
    prism.vertices[17] = (TerminalVertex){ .position = {{ 1.0f,  1.0f, -1.0f }}, .normal = {{ 0.0f,  1.0f, 0.0f }}, .color = (RGBcolor){ 128, 255, 0 } };

    TerminalTriangle tempTriangles[8] = {
        // Sides
        { .indices = { 0,  1,  2  } }, { .indices = { 0,  2,  3  } },
        { .indices = { 4,  5,  6  } }, { .indices = { 4,  6,  7  } },
        { .indices = { 8,  9,  10 } }, { .indices = { 8,  10, 11 } },
        // Caps
        { .indices = { 12, 13, 14 } }, 
        { .indices = { 15, 16, 17 } }  
    };

    for (size_t i = 0; i < 8; ++i)
    {
        prism.triangles[i] = tempTriangles[i];
    }

    prism.transform.position = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    prism.transform.rotation = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    prism.transform.scale    = (vec3s){{ 1.8f, 1.8f, 1.8f }};

    return prism;
}

int32_t main(void)
{
    terminalInit();

    TerminalFrame frame;
    frameCreate(&frame, TOP_LEFT_FRAME_POS, FULLSCREEN_FRAME_SIZE);
    frameSetFPS(&frame, 30);

    // Camera setup
    cameraSetPosition(&frame.camera, (vec3s){{ 0.0f, 1.5f, 10.0f }});
    cameraSetTarget(&frame.camera,    (vec3s){{ 0.0f, 0.0f, 0.0f }});

    // Ambient light floor
    frame.ambientColor = (RGBcolor){ .r = 30, .g = 30, .b = 30 };
    frame.ambientIntensity = 0.4f;

    // Directional Light (Sun)
    TerminalLight sun = lightCreateDirectional(
        (vec3s){{ 0.5f, -1.0f, -0.5f }}, 
        (RGBcolor){ .r = 220, .g = 230, .b = 255 }, 
        1.0f
    );
    frameAddDirectionalLight(&frame, sun);

    // Orbiting Point Light
    TerminalLight pointLight = lightCreatePoint(
        (vec3s){{ 0.0f, 0.0f, 0.0f }}, 
        (RGBcolor){ .r = 255, .g = 200, .b = 100 }, 
        1.5f
    );
    TerminalLightIndex pointLightIndex = frameAddPointLight(&frame, pointLight);

    // Create the three meshes
    TerminalMesh pyramid = createPyramidMesh();
    TerminalMesh centerCube = createCubeMesh();
    TerminalMesh prism = createPrismMesh();

    bool running      = true;
    bool paused       = false;
    bool wireframe    = false;
    bool rotateMesh   = true;
    int  renderMode   = 0;   // 0: Pyramid, 1: Prism, 2: Cube
    bool mKeyWasDown  = false;

    TerminalColor bg = terminalColorFromRGB(0, 0, 0);
    
    float orbitTime   = 0.0f;
    float meshRotTime = 0.0f;

    while (running)
    {
        terminalPollInput();

        if (terminalIsKeyDown(KEY_QUIT))
            running = false;
        if (terminalIsKeyDown(KEY_SPACE))
            paused = !paused;
        if (terminalIsKeyDown(KEY_A))
            wireframe = !wireframe;
        if (terminalIsKeyDown(KEY_D))
            rotateMesh = !rotateMesh;

        bool mIsDown = terminalIsKeyDown(KEY_S);
        if (mIsDown && !mKeyWasDown)
        {
            renderMode = (renderMode + 1) % 3;
        }
        mKeyWasDown = mIsDown;

        if (paused) continue;

        orbitTime += 0.03f;
        if (rotateMesh)
        {
            meshRotTime += 0.03f;
        }

        // --- Animations ---
        pyramid.transform.rotation    = (vec3s){{ meshRotTime * 0.5f, meshRotTime, 0.0f }};
        centerCube.transform.rotation = (vec3s){{ meshRotTime * 0.5f, meshRotTime, meshRotTime * 0.25f }};
        prism.transform.rotation      = (vec3s){{ 0.0f, -meshRotTime * 0.7f, meshRotTime * 0.3f }};

        float orbitRadius = 2.5f;
        float lightX = cosf(orbitTime * 1.2f) * orbitRadius;
        float lightZ = sinf(orbitTime * 1.2f) * orbitRadius;
        float lightY = sinf(orbitTime * 0.6f) * 0.5f;

        if (pointLightIndex != 0xFF)
        {
            frame.pointLights[pointLightIndex].position = (vec3s){{ lightX, lightY, lightZ }};
        }

        // --- Render Frame ---
        frameClear(&frame, bg);

        switch (renderMode)
        {
            case 0:
                frameDrawMesh(&frame, &pyramid, wireframe, SHADING_UNLIT);
                break;
            case 1:
                frameDrawMesh(&frame, &prism, wireframe, SHADING_FLAT);
                break;
            case 2:
                frameDrawMesh(&frame, &centerCube, wireframe, SHADING_SMOOTH);
                break;
        }

        frameRender(&frame);
    }

    // Cleanup
    meshDestroy(&pyramid);
    meshDestroy(&centerCube);
    meshDestroy(&prism);
    frameDestroy(&frame);
    terminalShutdown();

    return 0;
}
