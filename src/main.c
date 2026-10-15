#include <justLibrary.h>
#include <rinyTenderer/sample.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int32_t main(void)
{
    boot();
    terminalInit();

    FrameBuffer frame;
    frameBufferInit(&frame, TOP_LEFT_FRAME_POS, FULLSCREEN_FRAME_SIZE);
    frameBufferSetFPS(&frame, 30);

    bool running = true;

    // Triangle vertex positions
    TerminalPos a = { .x = 40, .y = 8  };
    TerminalPos b = { .x = 50, .y = 2  };
    TerminalPos c = { .x = 60, .y = 10 };

    int dx = 1;
    int dy = 1;
    float colorTime = 0.0f; // Used to smoothly cycle colors over time

    while (running)
    {
        terminalPollInput();

        if (terminalIsKeyDown(KEY_QUIT))
            running = false;

        // 1. Animate the triangle position
        a.x += dx; b.x += dx; c.x += dx;
        a.y += dy; b.y += dy; c.y += dy;

        // 2. Bounce off terminal boundaries (keeping a safety margin based on min/max coords)
        if (a.x <= 2 || b.x <= 2 || c.x <= 2 || 
            a.x >= frame.size.width - 2 || b.x >= frame.size.width - 2 || c.x >= frame.size.width - 2)
        {
            dx = -dx;
        }

        if (a.y <= 2 || b.y <= 2 || c.y <= 2 || 
            a.y >= frame.size.height - 2 || b.y >= frame.size.height - 2 || c.y >= frame.size.height - 2)
        {
            dy = -dy;
        }

        // 3. Smooth, vibrant color cycling using sine waves scaled to [0, 255]
        colorTime += 0.05f;
        uint8_t red   = (uint8_t)(sinf(colorTime + 0.0f) * 127.5f + 127.5f);
        uint8_t green = (uint8_t)(sinf(colorTime + 2.094f) * 127.5f + 127.5f); // +120 degrees offset
        uint8_t blue  = (uint8_t)(sinf(colorTime + 4.188f) * 127.5f + 127.5f); // +240 degrees offset

        // Map RGB to our ANSI 256 color palette
        TerminalPixel tri = terminalPixelFromColor('*', red, green, blue);

        // Background clear color (a very dark sleek charcoal gray)
        TerminalPixel bg = terminalPixelFromColor('&', 15, 15, 20);

        // 4. Clear back buffer before drawing this frame
        frameBufferClear(&frame, bg);

        // 5. Draw the triangle onto the back buffer
        frameBufferTriangle(&frame, a, b, c, tri);

        // 6. Render changes to screen (differential swap)
        frameBufferRender(&frame);
    }

    frameBufferDestroy(&frame);
    terminalShutdown();

    return 0;
}
