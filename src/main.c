#include <stdio.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>

int main(void)
{
    terminalInit();
    tuiInit();
    
    TerminalFrame gameFrame;
    frameCreateRelative(&gameFrame, 
                        (TerminalPosRel){{ 0.1f, 0.1f }}, 
                        (TerminalSizeRel){{ 0.8f, 0.8f }});

    // Define the color cycle array: Red -> Blue -> Green
    TerminalColor colorCycle[3] = {
        terminalColorFromRGB(255, 0, 0), // Red
        terminalColorFromRGB(0, 0, 255), // Blue
        terminalColorFromRGB(0, 255, 0)  // Green
    };
    int colorIndex = 0;
    TerminalColor clearColor = colorCycle[colorIndex];

    bool running = true;
    while (running)
    {
        terminalPollInput();
        if (terminalIsKeyDown(KEY_QUIT))
        {
            running = false;
        }
        else if (terminalIsKeyDown(KEY_SPACE))
        {
            // Trigger window size refresh, terminal clear, and resizes
            terminalUpdateSize();
            terminalClear();
            frameResize(&gameFrame);
            tuiResize();

            // Cycle to the next color: Red -> Blue -> Green -> Red...
            colorIndex = (colorIndex + 1) % 3;
            clearColor = colorCycle[colorIndex];
        }

        // - - - Render loop
        frameClear(&gameFrame, clearColor);
        tuiClear();

        // Draw TUI Rectangles
        TerminalColor lightGrey = terminalColorFromRGB(200, 200, 200);
        TerminalColor cyan      = terminalColorFromRGB(0, 200, 255);

        // Outer rounded panel
        tuiAddRect((TerminalPos){.x = 2, .y = 2}, (TerminalPos){.x = 50, .y = 20}, lightGrey, true);
        
        // Inner sharp box
        tuiAddRect((TerminalPos){.x = 5, .y = 4}, (TerminalPos){.x = 25, .y = 12}, cyan, false);

        // - - - Render and flush subsystems
        frameRender(&gameFrame);
        tuiRender();
    }

    // - - - Cleanup
    frameDestroy(&gameFrame);
    tuiDestroy();
    terminalShutdown();
    
    return 0;
}
