#include "rinyTenderer/renderer/light.h"
#include <justLibrary.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/renderer/mesh.h> // Ensure this points to your mesh header
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <cglm/cglm.h>
#include <cglm/struct.h>

// Helper to create a 3D cube mesh using your exact mesh3DCreate signature
static TerminalMesh createCubeMesh(void)
{
    terminalUpdateSize();
    justMemorySetLimit(1024 * 1024, "TERM_BUFFER");
    TerminalMesh cube;
    meshCreate(&cube, 8, 12);

// 1. Populate Vertices with standard RGB/CMY cube colors
    cube.vertices[0] = (TerminalVertex){ .position = {{ -1.0f, -1.0f, -1.0f }}, .color = (RGBcolor) {  0,   0,   0 } }; // Black
    cube.vertices[1] = (TerminalVertex){ .position = {{  1.0f, -1.0f, -1.0f }}, .color = (RGBcolor) {255,   0,   0 } }; // Red
    cube.vertices[2] = (TerminalVertex){ .position = {{  1.0f,  1.0f, -1.0f }}, .color = (RGBcolor) {255, 255,   0 } }; // Yellow
    cube.vertices[3] = (TerminalVertex){ .position = {{ -1.0f,  1.0f, -1.0f }}, .color = (RGBcolor) {  0, 255,   0 } }; // Green
    cube.vertices[4] = (TerminalVertex){ .position = {{ -1.0f, -1.0f,  1.0f }}, .color = (RGBcolor) {  0,   0, 255 } }; // Blue
    cube.vertices[5] = (TerminalVertex){ .position = {{  1.0f, -1.0f,  1.0f }}, .color = (RGBcolor) {255,   0, 255 } }; // Magenta
    cube.vertices[6] = (TerminalVertex){ .position = {{  1.0f,  1.0f,  1.0f }}, .color = (RGBcolor) {255, 255, 255 } }; // White
    cube.vertices[7] = (TerminalVertex){ .position = {{ -1.0f,  1.0f,  1.0f }}, .color = (RGBcolor) {  0, 255, 255 } }; // Cyan

    // 2. Populate Triangles (12 triangles, 2 per face)
    TerminalTriangle tempTriangles[12] = {
        { .indices = { 4, 5, 6 } }, { .indices = { 4, 6, 7 } }, // Front
        { .indices = { 1, 0, 3 } }, { .indices = { 1, 3, 2 } }, // Back
        { .indices = { 0, 4, 7 } }, { .indices = { 0, 7, 3 } }, // Left
        { .indices = { 5, 1, 2 } }, { .indices = { 5, 2, 6 } }, // Right
        { .indices = { 3, 7, 6 } }, { .indices = { 3, 6, 2 } }, // Top
        { .indices = { 0, 1, 5 } }, { .indices = { 0, 5, 4 } }  // Bottom
    };

    for (size_t i = 0; i < 12; ++i)
    {
        cube.triangles[i] = tempTriangles[i];
    }

    // 3. Initialize Transform defaults
    cube.transform.position = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    cube.transform.rotation = (vec3s){{ 0.0f, 0.0f, 0.0f }};
    cube.transform.scale    = (vec3s){{ 1.0f, 1.0f, 1.0f }};

    return cube;
}

int32_t main(void)
{
    terminalInit();

    TerminalFrame frame;
    frameCreate(&frame, TOP_LEFT_FRAME_POS, FULLSCREEN_FRAME_SIZE);
    frameSetFPS(&frame, 30);

    // Initial camera placement
    cameraSetPosition(&frame.camera, (vec3s){{ 0.0f, 1.5f, 4.5f }});
    cameraSetTarget(&frame.camera,   (vec3s){{ 0.0f, 0.0f, 0.0f }});

    TerminalLight sun = lightCreateDirectional((vec3s){{ 0.5f, -1.0f, -0.5f }}, (RGBcolor) { .r = 255, .g = 255, .b = 255 }, 1.0f);
    frameAddDirectionalLight(&frame, sun);

    TerminalMesh cube = createCubeMesh();

    bool running     = true;
    bool paused      = false;
    bool wireframe   = false;
    bool rotateCube  = true;  // Toggle with 'D'
    bool moveCamera  = true;  // Toggle with 'W'

    TerminalColor bg = terminalColorFromRGB(0, 0, 0);
    float timeTracker = 0.0f;
    float camAngle    = 0.0f;
    
    // Persistent offsets for smooth toggling
    float cubeRotX = 0.0f;
    float cubeRotY = 0.0f;
    float cubeRotZ = 0.0f;

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
            rotateCube = !rotateCube;
        if (terminalIsKeyDown(KEY_W))
            moveCamera = !moveCamera;

        if (paused) continue;

        // Increment animation time tickers conditionally
        if (rotateCube)
        {
            timeTracker += 0.04f;
            cubeRotX = timeTracker * 0.5f;
            cubeRotY = timeTracker;
            cubeRotZ = timeTracker * 0.25f;
        }

        if (moveCamera)
        {
            camAngle += 0.03f;
        }

        // 1. Update cube rotation transform
        cube.transform.rotation = (vec3s){{ cubeRotX, cubeRotY, cubeRotZ }};

        // 2. Camera Zooming (pulsing distance between close and far) + slow azimuth drift
        float minRadius = 2.0f;
        float maxRadius = 7.0f;
        // Smoothly oscillate radius using a sine wave
        float camRadius = minRadius + (maxRadius - minRadius) * 0.5f * (1.0f + sinf(camAngle * 0.6f));
        float azimuth   = camAngle * 0.25f; // Slow rotation to view different faces
        float camY      = 1.2f;             // Fixed slight elevation

        float camX = camRadius * cosf(azimuth);
        float camZ = camRadius * sinf(azimuth) + 2.0f;

        cameraSetPosition(&frame.camera, (vec3s){{ camX, camY, camZ }});
        cameraSetTarget(&frame.camera,   (vec3s){{ 0.0f, 0.0f, 0.0f }});

        // Clear frame color and depth buffers
        frameClear(&frame, bg);

        // Draw mesh through the depth-tested pipeline
        frameDrawMesh(&frame, &cube, wireframe);

        // Render frame to terminal
        frameRender(&frame);
    }

    // Cleanup using your exact destroy signature
    meshDestroy(&cube);
    frameDestroy(&frame);
    terminalShutdown();

    return 0;
}
