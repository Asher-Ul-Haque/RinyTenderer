#include <justLibrary.h>
#include <rinyTenderer/sample.h>
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
  justMemorySetLimit(1024 * 1024, "TERM_BUFFER");
    TerminalMesh cube;
    mesh3DCreate(&cube, 8, 12);

    // 1. Populate Vertices
    cube.vertices[0] = (TerminalVertex){ .position = {{ -1.0f, -1.0f, -1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[1] = (TerminalVertex){ .position = {{  1.0f, -1.0f, -1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[2] = (TerminalVertex){ .position = {{  1.0f,  1.0f, -1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[3] = (TerminalVertex){ .position = {{ -1.0f,  1.0f, -1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[4] = (TerminalVertex){ .position = {{ -1.0f, -1.0f,  1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[5] = (TerminalVertex){ .position = {{  1.0f, -1.0f,  1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[6] = (TerminalVertex){ .position = {{  1.0f,  1.0f,  1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };
    cube.vertices[7] = (TerminalVertex){ .position = {{ -1.0f,  1.0f,  1.0f }}, .color = terminalPixelFromColor('#', 0, 200, 255) };

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
    boot();
    terminalInit();

    TerminalFrame frame;
    frameInit(&frame, TOP_LEFT_FRAME_POS, FULLSCREEN_FRAME_SIZE);
    frameSetFPS(&frame, 30);

    // Position camera back from the origin
    cameraSetPosition(&frame.camera, (vec3s){{ 0.0f, 1.5f, 4.0f }});
    cameraSetTarget(&frame.camera,   (vec3s){{ 0.0f, 0.0f, 0.0f }});

    // Create cube using your exact API
    TerminalMesh cube = createCubeMesh();

    bool running = true;
    bool paused  = false;

    TerminalColor bg = terminalPixelFromColor(' ', 0, 0, 0);
    float rotationAngle = 0.0f;

    while (running)
    {
        terminalPollInput();

        if (terminalIsKeyDown(KEY_QUIT))
            running = false;
        if (terminalIsKeyDown(KEY_SPACE))
            paused = !paused;

        if (paused) continue;

        // Animate rotation
        rotationAngle += 0.03f;
        cube.transform.rotation = (vec3s){{ rotationAngle * 0.5f, rotationAngle, 0.0f }};

        // Clear frame color and depth buffers
        frameClear(&frame, bg);

        // Draw mesh through the depth-tested pipeline
        frameDrawMesh(&frame, &cube, false);

        // Render frame to terminal
        frameRender(&frame);
    }

    // Cleanup using your exact destroy signature
    mesh3DDestroy(&cube);
    frameDestroy(&frame);
    terminalShutdown();

    return 0;
}
