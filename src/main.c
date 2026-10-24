#include <stdio.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>

int main(void)
{
    terminalInit();
    
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
            // Trigger window size refresh, terminal clear, and frame resize
            terminalUpdateSize();
            terminalClear();
            frameResize(&gameFrame);

            // Cycle to the next color: Red -> Blue -> Green -> Red...
            colorIndex = (colorIndex + 1) % 3;
            clearColor = colorCycle[colorIndex];
        }

        // - - - Render loop
        frameClear(&gameFrame, clearColor);

        // Draw some test geometry or pixels here if desired...

        frameRender(&gameFrame);
    }

    // - - - Cleanup
    frameDestroy(&gameFrame);
    terminalShutdown();
    
    return 0;
}
