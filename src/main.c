#include <justLibrary.h>
#include <rinyTenderer/sample.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdio.h>
#include <unistd.h>

int32_t main(void)
{
  if (isatty(STDIN_FILENO)) {
        printf("stdin IS a valid TTY\n");
    } else {
        printf("CRITICAL: stdin is NOT a TTY!\n");
    }
    fflush(stdout);
    sleep(5);
    boot();
    terminalInit();

    // 1. Define a viewport size (e.g., 40 columns wide by 10 rows high)
    // Position it at row 3, col 5 on the screen
    TerminalSize frameSize = { .rows = 10, .cols = 40 };
    TerminalSize framePos  = { .rows = 3,  .cols = 5  };

    FrameBuffer frame;
    frameBufferInit(&frame, framePos, frameSize);

    // 2. Draw some test characters using 0-based local coordinates inside the frame
    // Let's draw an 'X' at the top-left and bottom-right corners
    frameBufferSetChar(&frame, (TerminalSize){ .rows = 0, .cols = 0 });
    frameBufferSetChar(&frame, (TerminalSize){ .rows = 9, .cols = 39 });
    
    // Draw a horizontal line of '#' characters across row 5
    for (uint16_t c = 0; c < 40; ++c)
    {
        frameBufferSetChar(&frame, (TerminalSize){ .rows = 5, .cols = c });
    }

    // 3. Render the entire frame buffer in one clean write call
    frameBufferRender(&frame);
    fflush(stdout);

    sleep(3); // Wait 3 seconds to see the frame

    // 4. Clean up frame memory and terminal state
    frameBufferDestroy(&frame);

    terminalShutdown();
    return 0;
}
