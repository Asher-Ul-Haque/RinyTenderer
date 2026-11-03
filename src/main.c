#include "rinyTenderer/_structs.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/renderer/mesh.h>
#include <rinyTenderer/renderer/light.h>
#include <rinyTenderer/renderer/primitiveGeometry.h>
#include <rinyTenderer/renderer/camera.h>

int main(void)
{
    terminalInit();
    
    // Create a fullscreen-ish frame
    TerminalFrame gameFrame;
    frameCreateRelative(&gameFrame, 
                        (TerminalPosRel){ .x = 0.05f, .y = 0.05f }, 
                        (TerminalSizeRel){ .x = 0.9f, .y = 0.9f });

    // Setup camera perspective
    cameraSetTarget(&gameFrame.camera, (vec3s){ .x = 0.0f, .y = 0.0f, .z = 0.0f });
    cameraSetPerspective(&gameFrame.camera, 60.0f, 0.1f, 100.0f);

    // --- Create a Single Triangle Mesh ---
    TerminalMesh triMesh;
    meshCreate(&triMesh, 3, 1);

    // Define vertices for a centered triangle with distinct vertex colors
    triMesh.vertices[0] = (TerminalVertex){ .position = { .x =  0.0f, .y =  0.8f, .z = 0.0f }, .normal = { .x = 0.0f, .y = 0.0f, .z = 1.0f }, .color = { 255, 50, 50 } };   // Top: Red
    triMesh.vertices[1] = (TerminalVertex){ .position = { .x = -0.8f, .y = -0.8f, .z = 0.0f }, .normal = { .x = 0.0f, .y = 0.0f, .z = 1.0f }, .color = { 50, 255, 50 } };   // Bottom-Left: Green
    triMesh.vertices[2] = (TerminalVertex){ .position = { .x =  0.8f, .y = -0.8f, .z = 0.0f }, .normal = { .x = 0.0f, .y = 0.0f, .z = 1.0f }, .color = { 50, 50, 255 } };   // Bottom-Right: Blue

    triMesh.triangles[0].indices[0] = 0;
    triMesh.triangles[0].indices[1] = 1;
    triMesh.triangles[0].indices[2] = 2;

    // Frame pacing configuration (~60 FPS -> 16.6 ms)
    struct timespec frameDelay = { .tv_sec = 0, .tv_nsec = 16666666L };
    TerminalColor clearColor = terminalColorFromRGB(20, 20, 30);

    float angle = 0.0f;
    bool running = true;
    while (running)
    {
        terminalPollInput();
        if (terminalIsKeyDown(KEY_QUIT))
        {
            running = false;
        }

        // --- Orbit Camera Around Origin ---
        angle += 0.03f;
        float radius = 3.0f;
        float camX = sinf(angle) * radius;
        float camZ = cosf(angle) * radius;
        cameraSetPosition(&gameFrame.camera, (vec3s){ .x = camX, .y = 0.0f, .z = camZ });

        // --- Render Pipeline ---
        frameClear(&gameFrame, clearColor);

        // Draw the single mesh using unlit shading
        frameDrawMesh(&gameFrame, &triMesh, false, SHADING_UNLIT);

        // Draw title string character by character
        TerminalColor titleFg = terminalColorFromRGB(255, 255, 255);
        TerminalColor titleBg = terminalColorFromRGB(60, 60, 90);
        const char* title = "RINY 3D CAMERA ORBIT";
        for (int i = 0; title[i] != '\0'; i++)
        {
            char cStr[2] = { title[i], '\0' };
            FramePos charPos = { .x = 2.0f + (float)i, .y = 1.0f, .z = 0.0f };
            frameSetChar(&gameFrame, charPos, cStr, titleFg, titleBg);
        }

        // Flush frame to terminal output
        frameRender(&gameFrame);

        // Pace frame rate
        nanosleep(&frameDelay, NULL);
    }

    // --- Cleanup ---
    meshDestroy(&triMesh);
    frameDestroy(&gameFrame);
    terminalShutdown();
    
    return 0;
}
