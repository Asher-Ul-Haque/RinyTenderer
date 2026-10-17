#include <bits/time.h>
#include <justLibrary.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


// - - - Color - - -

JUST_API TerminalColor terminalPixelFromColor(char CHAR, uint8_t R, uint8_t G, uint8_t B)
{
  // - - - Check if the color is close to grayscale
  if (R == G && G == B)
  {
    // - - - Pure black / white extremes
    if (R < 8)   return 16;
    if (R > 248) return 231;
    
    // - - - Map to the 24-step grayscale ramp (codes 232 - 255) Formula: 232 + (brightness step from 0 to 23)
    return 232 + (R - 8) * 24 / 247;
  }

  // - - - Otherwise, map to the 6x6x6 color cube (codes 16 - 231)
  // - - - Scale R, G, B from [0, 255] down to [0, 5]
  int ir = (R * 5) / 255;
  int ig = (G * 5) / 255;
  int ib = (B * 5) / 255;

  // - - - Cube formula: 16 + 36 * r + 6 * g + b
  return (uint8_t)(16 + (36 * ir) + (6 * ig) + ib);
}


// - - - Create and Destroy - - - 

JUST_API void frameBufferInit(FrameBuffer* FRAME, TerminalSize POSITION, TerminalSize SIZE)
{
  JUST_LOG_DEBUG("[FRAME] : Creating a new frame");

  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot init a NULL FRAME");

  const TerminalCTX* ctx = terminalGetContext();

  if (SIZE.cols == 0 || SIZE.rows == 0)
  {
    JUST_LOG_WARNING("[FRAME] : Going for maximimum size");
    FRAME->size = ctx->size;
  }
  else
  {
    FRAME->size = SIZE;
  }

  if (POSITION.rows == 0 || POSITION.cols == 0)
  {
    FRAME->position = TOP_LEFT_FRAME_POS;
  }
  else
  {
    FRAME->position = POSITION;
  }

  // - - - allocate 1D buffer : ((width + 1 for \n) * height) + 1 for \0
  size_t width      = FRAME->size.width;
  size_t height     = FRAME->size.height;
  size_t totalCells = width * height;
  
  JUST_LOG_DEBUG(
    "[FRAME] : Size = %u cols x %u rows",
    (unsigned)FRAME->size.width,
    (unsigned)FRAME->size.height);

  FRAME->backBuffer = (TerminalColor*) JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor), "TERM_BUFFER");
  JUST_ASSERT_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  FRAME->frontBuffer = (TerminalColor*) JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor), "TERM_BUFFER");
  JUST_ASSERT_MESSAGE(FRAME->frontBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  // - - - Populate buffer with spaces, newlines at row ends and \0 at the end
  for (uint16_t r = 0; r < height; ++r)
  {
    size_t rowOffset = r * width;
    for (uint16_t c = 0; c < width; ++c)
    {
      FRAME->backBuffer[rowOffset + c]  = 0;
      FRAME->frontBuffer[rowOffset + c] = 255;
    }
  }

  // - - - setup fps
  clock_gettime(CLOCK_MONOTONIC, &FRAME->lastFrameTime);
  FRAME->targetFPS = 30;
  FRAME->actualFPS = 0;

  // - - - initial clear
  frameBufferClear(FRAME, 0);

  JUST_LOG_INFO("[FRAME] : Frame ready");
}

JUST_API void frameBufferDestroy(FrameBuffer* FRAME)
{
  JUST_LOG_DEBUG("[FRAME] : Destroying a FRAME");

  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot destroy a NULL FRAME");

  JUST_FREE(FRAME->backBuffer);
  JUST_FREE(FRAME->frontBuffer);
  FRAME->backBuffer  = NULL;
  FRAME->frontBuffer = NULL;

  JUST_LOG_INFO("[FRAME] : Destroyed");
}


// - - - Render and clear - - - 

JUST_API void frameBufferRender(FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot render in an uninitialized buffer");

  // - - - scan through and only write cells that changed
  uint16_t width  = FRAME->size.width;
  uint16_t height = FRAME->size.height;
  uint16_t rows   = height / 2;

  for (uint16_t y = 0; y < rows; ++y)
  {
    for (uint16_t x = 0; x < width; ++x)
    {
      size_t topIndex     = (y * 2)     * width + x;
      size_t bottomIndex  = (y * 2 + 1) * width + x;

      TerminalColor topBack     = FRAME->backBuffer[topIndex];
      TerminalColor bottomBack  = FRAME->backBuffer[bottomIndex];

      TerminalColor topFront    = FRAME->frontBuffer[topIndex];
      TerminalColor bottomFront = FRAME->frontBuffer[bottomIndex];

      if (topBack != topFront || bottomBack != bottomFront)
      {
      char ch[4] = " ";
      uint8_t fg = 7;
      uint8_t bg = 0;

      bool hasTop = (topBack != 0);
      bool hasBot = (bottomBack != 0);

      if (hasTop && hasBot)
      {
        snprintf(ch, sizeof(ch), "▀");
        fg = topBack;
        bg = bottomBack;
      }
      else if (hasTop && !hasBot)
      {
        snprintf(ch, sizeof(ch), "▀");
        fg = topBack;
        bg = 0;
      }
      else if (!hasTop && hasBot)
      {
        snprintf(ch, sizeof(ch), "▄");
        fg = bottomBack;
        bg = 0;
      }
      else
      {
        snprintf(ch, sizeof(ch), " ");
        fg = 0;
        bg = 0;
      }

      printf("\033[38;5;%dm\033[48;5;%dm\033[%d;%dH%s", 
             fg, bg,
             FRAME->position.rows + y, 
             FRAME->position.cols + x, 
             ch);

        // - - - Sync front buffer
        FRAME->frontBuffer[topIndex]    = topBack;
        FRAME->frontBuffer[bottomIndex] = bottomBack;
      }
    }
  }
  fflush(stdout);

  // - - - update timer
struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);

    uint64_t targetNs  = FRAME->targetFPS > 0 ? 1000000000L / FRAME->targetFPS : 1;
    uint64_t elapsedNs = (currentTime.tv_sec  - FRAME->lastFrameTime.tv_sec) * 1000000000L +
                         (currentTime.tv_nsec - FRAME->lastFrameTime.tv_nsec);

    if (elapsedNs == 0) elapsedNs = 1;
    uint8_t instantFPS  = (uint8_t)(1000000000ULL / elapsedNs);
    FRAME->actualFPS    = (FRAME->actualFPS == 0) ? instantFPS : (uint8_t)((FRAME->actualFPS * 4 + instantFPS) / 5);

    if (elapsedNs < targetNs)
    {
      struct timespec sleepTime = 
        { 
          .tv_sec = 0, 
          .tv_nsec = targetNs - elapsedNs 
        };
      nanosleep(&sleepTime, NULL);
    }
    clock_gettime(CLOCK_MONOTONIC, &FRAME->lastFrameTime);

  #ifdef DEBUG
    static uint8_t frame = 0;
    frame++;
    if (frame % 30 == 0)
    {
      JUST_LOG_DEBUG("FPS : %d", FRAME->actualFPS);
    }
  #endif 
}

JUST_API void frameBufferClear(FrameBuffer* FRAME, TerminalColor PIXEL_TEMPLATE)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot clear an uninitialized buffer");

  for (uint16_t y = 0; y < FRAME->size.height; ++y)
  {
    for (uint16_t x = 0; x < FRAME->size.width; ++x)
    {
      size_t index              = y * (FRAME->size.width) + x;
      FRAME->backBuffer[index]  = PIXEL_TEMPLATE;
    }
  }
}

JUST_API void frameBufferSetPixel(FrameBuffer* FRAME, TerminalPos POSITION, TerminalColor PIXEL)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot draw in an uninitialized buffer");

  if (POSITION.x >= FRAME->size.width)  return;
  if (POSITION.y >= FRAME->size.height) return;

  size_t index              = POSITION.y * (FRAME->size.width) + POSITION.x;
  FRAME->backBuffer[index]  = PIXEL;
}


// - - - Geometry - - - 

JUST_API void frameBufferLine(
    FrameBuffer*  FRAME, 
    TerminalPos   START_POS, 
    TerminalPos   END_POS, 
    TerminalColor PIXEL_TEMPLATE)
{
    int x0 = START_POS.x;
    int y0 = START_POS.y;
    int x1 = END_POS.x;
    int y1 = END_POS.y;

    // - - - Handle steep lines by swapping the axes.
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep)
    {
      int temp  = x0;
      x0        = y0;
      y0        = temp;

      temp      = x1;
      x1        = y1;
      y1        = temp;
    }

    // - - - Iterate from left to right.
    if (x0 > x1)
    {
      int temp  = x0;
      x0        = x1;
      x1        = temp;

      temp      = y0;
      y0        = y1;
      y1        = temp;
    }

    int dx    = x1 - x0;
    int dy    = abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y     = y0;

  for (int x = x0; x <= x1; x++)
  {
    TerminalSize position = 
    {
      .x = steep ? y : x,
      .y = steep ? x : y
    };
    frameBufferSetPixel(FRAME, position, PIXEL_TEMPLATE);

    error -= dy;
    if (error < 0)
    {
      y     += ystep;
      error += dx;
    }
  }
}


JUST_API void frameBufferTriangle(
    FrameBuffer*  FRAME, 
    TerminalPos   POINT_A, 
    TerminalPos   POINT_B, 
    TerminalPos   POINT_C, 
    TerminalColor PIXEL_TEMPLATE)
{
  int ax = POINT_A.x;
  int ay = POINT_A.y;
  int bx = POINT_B.x;
  int by = POINT_B.y;
  int cx = POINT_C.x;
  int cy = POINT_C.y;

  // - - - bubble sort
  if (ay > by)
  {
    int temp  = ax;
    ax        = bx;
    bx        = temp;

    temp  = ay;
    ay    = by;
    by    = temp;
  }
  if (ay > cy)
  {
    int temp  = ax;
    ax        = cx;
    cx        = temp;

    temp  = ay;
    ay    = cy;
    cy    = temp;
  }
  if (by > cy)
  {
    int temp  = bx;
    bx        = cx;
    cx        = temp;

    temp  = by;
    by    = cy;
    cy    = temp;
  }
  int height = cy - ay;

  // - - - bottom half
  if (ay != by)
  {
    // - - - swwp a horizontal line from ay to by
    int segmentHeight = by - ay;
    for (int y = ay; y <= by; ++y)
    {
      int x1        = ax + ((cx - ax) * (y - ay)) / height;
      int x2        = ax + ((bx - ax) * (y - ay)) / segmentHeight;

      int minX, maxX;
      if (x1 > x2)
      {
        minX = x2;
        maxX = x1;
      }
      else
      {
        minX = x1;
        maxX = x2;
      }

      for (int x = minX; x <= maxX; ++x)
      {
        frameBufferSetPixel(FRAME, (TerminalPos){.x = x, .y = y}, PIXEL_TEMPLATE);
      }
    }
  }

  // - - - top half
  if (by != cy)
  {
    // - - - swwp a horizontal line from ay to by
    int segmentHeight = cy - by;
    for (int y = by; y <= cy; ++y)
    {
      int x1        = ax + ((cx - ax) * (y - ay)) / height;
      int x2        = bx + ((cx - bx) * (y - by)) / segmentHeight;

      int minX, maxX;
      if (x1 > x2)
      {
        minX = x2;
        maxX = x1;
      }
      else
      {
        minX = x1;
        maxX = x2;
      }

      for (int x = minX; x <= maxX; ++x)
      {
        frameBufferSetPixel(FRAME, (TerminalPos){.x = x, .y = y}, PIXEL_TEMPLATE);
      }
    }
  }
}
