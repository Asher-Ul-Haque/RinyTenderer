#include "rinyTenderer/_structs.h"
#include "rinyTenderer/tui/tui.h"
#include <stdio.h>
#include <time.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/renderer/mesh.h>
#include <rinyTenderer/renderer/light.h>
#include <rinyTenderer/renderer/primitiveGeometry.h>
#include <rinyTenderer/renderer/camera.h>

int main(void)
{
    terminalInit();
    
    TerminalFrame mainFrame;
    frameCreateRelative(&mainFrame, 
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
        frameClear(&mainFrame, clearColor);

// Interactive Widget States
    bool checkboxState1 = true;
    bool checkboxState2 = false;
    float sliderValueFOV = 65.0f;
    float sliderValueVolume = 0.8f;
    float progressVal = 0.45f;

    bool running = true;
    while (running)
    {
        terminalPollInput();

        if (terminalIsKeyDown(KEY_Q))
        {
            running = false;
        }

        // Clear frame buffer before redrawing
        frameClear(&mainFrame, 0);

        // 1. Draw Container Panel / Box
        tuiDrawBox(&mainFrame, (TuiBox){
            .topLeft     = { .x = 2.0f,  .y = 2.0f,  .z = 0.0f },
            .bottomRight = { .x = 74.0f, .y = 22.0f, .z = 0.0f },
            .color       = terminalColorFromRGB(0, 255, 255),
            .boxType     = TUI_BORDER_ROUNDED
        });

        // 2. Draw Title Text Header
        tuiDrawText(&mainFrame, (TuiText){
            .position = { .x = 4.0f, .y = 3.0f, .z = 1.0f },
            .text     = "=== RINY TENDERER TUI DASHBOARD ===",
            .maxLen   = 36,
            .width    = 36,
            .fgColor  = terminalColorFromRGB(255, 255, 255),
            .bgColor  = TERMINAL_COLOR_TRANSPARENT
        });

        // 3. Interactive Checkboxes
        tuiCheckbox(&mainFrame, (TuiCheckbox){
            .position  = { .x = 6.0f, .y = 6.0f, .z = 2.0f },
            .label     = "Enable VSync / Frame Lock",
            .checked   = &checkboxState1,
            .boxColor  = terminalColorFromRGB(0, 255, 0),
            .textColor = terminalColorFromRGB(255, 255, 255)
        });

        tuiCheckbox(&mainFrame, (TuiCheckbox){
            .position  = { .x = 6.0f, .y = 9.0f, .z = 2.0f },
            .label     = "Wireframe Debug Mode",
            .checked   = &checkboxState2,
            .boxColor  = terminalColorFromRGB(255, 255, 0),
            .textColor = terminalColorFromRGB(255, 255, 255)
        });

        // 4. Interactive Sliders
        tuiSlider(&mainFrame, (TuiSlider){
            .position     = { .x = 6.0f, .y = 12.0f, .z = 2.0f },
            .length       = 25,
            .orientation  = TUI_HORIZONTAL,
            .label        = "FOV",
            .maxLen       = 3,
            .value        = &sliderValueFOV,
            .minValue     = 30.0f,
            .maxValue     = 120.0f,
            .fillColor    = terminalColorFromRGB(0, 0, 255),
            .emptyColor   = terminalColorFromRGB(0, 0, 0),
            .accentColor  = terminalColorFromRGB(255, 255, 255),
            .showValue    = true
        });

        tuiSlider(&mainFrame, (TuiSlider){
            .position     = { .x = 6.0f, .y = 15.0f, .z = 2.0f },
            .length       = 25,
            .orientation  = TUI_HORIZONTAL,
            .label        = "Vol",
            .maxLen       = 3,
            .value        = &sliderValueVolume,
            .minValue     = 0.0f,
            .maxValue     = 1.0f,
            .fillColor    = terminalColorFromRGB(255, 0, 255),
            .emptyColor   = terminalColorFromRGB(0, 0, 0),
            .accentColor  = terminalColorFromRGB(255, 255, 255),
            .showValue    = true
        });

        // 5. Progress Bar
        tuiDrawProgressBar(&mainFrame, (TuiProgressBar){
            .position       = { .x = 42.0f, .y = 6.0f, .z = 2.0f },
            .length         = 24,
            .orientation    = TUI_HORIZONTAL,
            .progress       = progressVal,
            .fillColor      = terminalColorFromRGB(0, 255, 0),
            .emptyColor     = 0,
            .textColor      = terminalColorFromRGB(255, 255, 255),
            .showPercentage = true
        });

        // 6. Action Button
        bool buttonClicked = tuiButton(&mainFrame, (TuiButton){
            .topLeft     = { .x = 42.0f, .y = 10.0f, .z = 2.0f },
            .bottomRight = { .x = 68.0f, .y = 16.0f, .z = 2.0f },
            .borderType  = TUI_BORDER_HEAVY,
            .fillColor   = terminalColorFromRGB(45, 120, 60),
            .accentColor = terminalColorFromRGB(255, 255, 0),
            .text        = "EXECUTE ACTION",
            .maxLen      = 14
        });

        if (buttonClicked)
        {
            // Trigger feedback or update state when clicked
            progressVal += 0.1f;
            if (progressVal > 1.0f) progressVal = 0.0f;
        }

        frameRender(&mainFrame);
        nanosleep(&frameDelay, NULL);
    }

    frameDestroy(&mainFrame);
    terminalShutdown();
    
    return 0;
    }
}
