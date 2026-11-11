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
    
    TerminalFrame gameFrame;
    frameCreateRelative(&gameFrame, 
                        (TerminalPosRel){ .x = 0.05f, .y = 0.05f }, 
                        (TerminalSizeRel){ .x = 0.9f, .y = 0.9f });

    struct timespec frameDelay = { .tv_sec = 0, .tv_nsec = 16666666L };
    TerminalColor clearColor = terminalColorFromRGB(20, 20, 30);

    bool running = true;
    while (running)
    {
        terminalPollInput();
        if (terminalIsKeyDown(KEY_Q))
        {
            running = false;
        }

        // --- Render Pipeline ---
        frameClear(&gameFrame, clearColor);

        TerminalPos mouse = terminalGetMousePos();
        char mouseStr[64];
        snprintf(mouseStr, sizeof(mouseStr), "Mouse Screen: X:%d Y:%d | Click: %s", 
                 (int)mouse.x, (int)mouse.y, 
                 terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT) ? "DOWN" : "UP");

        tuiDrawText(&gameFrame, (TuiText){
            .position = { .x = 4.0f, .y = 2.0f, .z = 9.0f },
            .text = mouseStr,
            .maxLen = sizeof(mouseStr),
            .fgColor = 220,
            .bgColor = TERMINAL_COLOR_TRANSPARENT
        });

        TuiButton bigButton = {
            .fillColor   = 52,                    
            .accentColor = 46,                    
            .topLeft     = { .x = 10.0f, .y = 10.0f, .z = 5.0f },
            .bottomRight = { .x = 26.0f, .y = 26.0f, .z = 5.0f },
            .borderType  = DOUBLE,
            .text        = "CLICK ME TO LOG",
            .maxLen      = 18
        };

        if (tuiButton(&gameFrame, bigButton))
        {
            JUST_LOG_INFO("[APP] : Big Button Pressed Successfully at Mouse X:%d Y:%d!", (int)mouse.x, (int)mouse.y);
        }
        else
        {
            if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                JUST_LOG_INFO("[APP] : Mouse Down, but missed button at X:%d Y:%d", (int)mouse.x, (int)mouse.y);
            }
        }

        frameRender(&gameFrame);
        nanosleep(&frameDelay, NULL);
    }

    frameDestroy(&gameFrame);
    terminalShutdown();
    
    return 0;
}
