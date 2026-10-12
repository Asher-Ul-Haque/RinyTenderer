#include <justLibrary.h>
#include <rinyTenderer/sample.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdio.h>
#include <unistd.h>

int32_t main(void)
{
    boot();
    terminalInit();

    FrameBuffer frame;
    frameBufferInit(&frame, TOP_LEFT_FRAME_POS, FULLSCREEN_FRAME_SIZE);

    // 1. Diagonal line
    frameBufferLine(&frame,
        (TerminalSize){ .x = 2, .y = 2 },
        (TerminalSize){ .x = 20, .y = 12 });

    // 2. Reverse diagonal
    frameBufferLine(&frame,
        (TerminalSize){ .x = 20, .y = 2 },
        (TerminalSize){ .x = 2, .y = 12 });

    // 3. Rectangle
    frameBufferLine(&frame,
        (TerminalSize){ .x = 30, .y = 3 },
        (TerminalSize){ .x = 50, .y = 3 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 30, .y = 3 },
        (TerminalSize){ .x = 30, .y = 13 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 30, .y = 13 },
        (TerminalSize){ .x = 50, .y = 13 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 50, .y = 3 },
        (TerminalSize){ .x = 50, .y = 13 });

    // 4. Triangle
    frameBufferLine(&frame,
        (TerminalSize){ .x = 60, .y = 12 },
        (TerminalSize){ .x = 70, .y = 2 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 70, .y = 2 },
        (TerminalSize){ .x = 80, .y = 12 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 60, .y = 12 },
        (TerminalSize){ .x = 80, .y = 12 });

    // 5. Diamond
    frameBufferLine(&frame,
        (TerminalSize){ .x = 95, .y = 2 },
        (TerminalSize){ .x = 105, .y = 8 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 105, .y = 8 },
        (TerminalSize){ .x = 95, .y = 14 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 95, .y = 14 },
        (TerminalSize){ .x = 85, .y = 8 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 85, .y = 8 },
        (TerminalSize){ .x = 95, .y = 2 });

    // 6. Star / cross
    frameBufferLine(&frame,
        (TerminalSize){ .x = 15, .y = 20 },
        (TerminalSize){ .x = 15, .y = 35 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 5, .y = 27 },
        (TerminalSize){ .x = 25, .y = 27 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 8, .y = 21 },
        (TerminalSize){ .x = 22, .y = 33 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 22, .y = 21 },
        (TerminalSize){ .x = 8, .y = 33 });

    // 7. Parallel lines
    for (int i = 0; i < 5; i++)
    {
        frameBufferLine(&frame,
            (TerminalSize){ .x = 35, .y = 20 + i * 3 },
            (TerminalSize){ .x = 55, .y = 20 + i * 3 });
    }

    // 8. A small zigzag
    frameBufferLine(&frame,
        (TerminalSize){ .x = 65, .y = 22 },
        (TerminalSize){ .x = 72, .y = 30 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 72, .y = 30 },
        (TerminalSize){ .x = 79, .y = 22 });

    frameBufferLine(&frame,
        (TerminalSize){ .x = 79, .y = 22 },
        (TerminalSize){ .x = 86, .y = 30 });

    frameBufferRender(&frame);
    sleep(3);

    frameBufferDestroy(&frame);
    terminalShutdown();

    return 0;
}
