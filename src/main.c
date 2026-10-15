#include <justLibrary.h>
#include <rinyTenderer/sample.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdio.h>
#include <time.h>
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

    TerminalPos a2 = { .x = 15, .y = 4  };
    TerminalPos b2 = { .x = 5,  .y = 20 };
    TerminalPos c2 = { .x = 35, .y = 18 };

    clock_t start = clock();

      frameBufferTriangleFilled(&frame, a, b, c);
      frameBufferTriangleFilled(&frame, a2, b2, c2);

    clock_t end = clock();
    double elapsed_ms = (double) (end - start) * 1000.0 / CLOCKS_PER_SEC;
    JUST_LOG_DEBUG("TOOK : %.2f ms\n", elapsed_ms);

    bool running = true;
    while (running)
    {
      terminalPollInput();
      if (terminalIsKeyDown(KEY_QUIT)) running = false;

      frameBufferRender(&frame);
      struct timespec ts = {
          .tv_sec = 0,
          .tv_nsec = 500000000 // 500,000,000 ns = 500,000 us = 0.5 seconds
      };
      nanosleep(&ts, NULL);
    }

    frameBufferDestroy(&frame);
    terminalShutdown();

    return 0;
}
