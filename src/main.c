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


    TerminalPos a = { .x = 60, .y = 12 };
    TerminalPos b = { .x = 70, .y = 2  };
    TerminalPos c = { .x = 80, .y = 15 };
    frameBufferTriangleFilled(&frame, a, b, c);

    TerminalPos a2 = { .x = 15, .y = 4  };
    TerminalPos b2 = { .x = 5,  .y = 20 };
    TerminalPos c2 = { .x = 35, .y = 18 };
    frameBufferTriangleFilled(&frame, a2, b2, c2);


    frameBufferRender(&frame);
    sleep(3);

    frameBufferDestroy(&frame);
    terminalShutdown();

    return 0;
}
