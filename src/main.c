#include "rinyTenderer/_structs.h"
#include "rinyTenderer/tui/tui.h"
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
    int dir = 1;
    while (running)
    {
        terminalPollInput();
        if (terminalIsKeyDown(KEY_Q))
        {
            running = false;
        }

        // --- Orbit Camera Around Origin ---
        angle += 0.03f * dir;
        float radius = 3.0f;
        float camX = sinf(angle) * radius;
        float camZ = cosf(angle) * radius;
        cameraSetPosition(&gameFrame.camera, (vec3s){ .x = camX, .y = 0.0f, .z = camZ });

        if (angle > 1 || angle < 0) dir = -dir;

        // --- Render Pipeline ---
        frameClear(&gameFrame, clearColor);

        // Draw the single mesh using unlit shading
        frameDrawMesh(&gameFrame, &triMesh, false, SHADING_UNLIT);

        // - - - Add TUI - - -
        
        // - - - Add a box
        FramePos boxTopLeft  = { .x = 4.0f, .y = 2.0f, .z = 10.0f };
        FramePos boxBotRight = { .x = 60.0f, .y = 20.0f, .z = 10.0f };
        tuiDrawBox(&gameFrame, boxTopLeft, boxBotRight, 51, ROUNDED);

        // - - - Add a title
        FramePos titlePos = { .x = 6.0f, .y = 4.0f, .z = 9.0f };
        tuiDrawTextCentered(&gameFrame, titlePos, 52, "RINY TENDERER - TUI DEMO", 25, 231, TERMINAL_COLOR_TRANSPARENT);

        // - - - Add a regular label
        FramePos labelPos = { .x = 8.0f, .y = 8.0f, .z = 9.0f };
        tuiDrawText(&gameFrame, labelPos, "Status: Running smoothly", 24, 46, TERMINAL_COLOR_TRANSPARENT);

        // - - - Add mosue pos:
        TerminalPos mouse = terminalGetMousePos();
        char mouseStr[64];
        snprintf(mouseStr, sizeof(mouseStr), "Mouse: X:%d Y:%d | Click: %s", 
                 (int)mouse.x, (int)mouse.y, 
                 terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT) ? "DOWN" : "UP");

        FramePos mouseLabelPos = { .x = 8.0f, .y = 15.0f, .z = 9.0f };
        tuiDrawText(&gameFrame, mouseLabelPos, mouseStr, sizeof(mouseStr), 220, TERMINAL_COLOR_TRANSPARENT);

        // - - - Add a dynamic progress bar
        FramePos barPos = { .x = 8.0f, .y = 12.0f, .z = 9.0f };
        tuiDrawProgressBar(&gameFrame, barPos, 44, angle, 40, 238);

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
