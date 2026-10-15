#include <bits/time.h>
#include <justLibrary.h>
#include <math.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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
  size_t totalSize  = ((width + 1) * height) + 1;
  
  JUST_LOG_DEBUG(
    "[FRAME] : Size = %u cols x %u rows",
    (unsigned)FRAME->size.width,
    (unsigned)FRAME->size.height);

  FRAME->backBuffer = (char*) JUST_MALLOC_TAGGED(totalSize, "TERM_BUFFER");
  JUST_ASSERT_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  FRAME->frontBuffer = (char*) JUST_MALLOC_TAGGED(totalSize, "TERM_BUFFER");
  JUST_ASSERT_MESSAGE(FRAME->frontBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  // - - - Populate buffer with spaces, newlines at row ends and \0 at the end
  for (uint16_t r = 0; r < height; ++r)
  {
    size_t rowOffset = r * (width + 1);
    for (uint16_t c = 0; c < width; ++c)
    {
      FRAME->backBuffer[rowOffset + c] = ' ';
    }
    FRAME->backBuffer[rowOffset + width] = '\n';
  }
  FRAME->backBuffer[totalSize - 1] = '\0';

  // - - - setup fps
  clock_gettime(CLOCK_MONOTONIC, &FRAME->lastFrameTime);
  FRAME->targetFPS = 0;
  FRAME->actualFPS = 0;

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

JUST_API void frameBufferSetFPS(FrameBuffer* FRAME, uint8_t TARGET_FPS)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  FRAME->targetFPS = TARGET_FPS;
}

JUST_API uint8_t frameBufferGetFPS(const FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  return FRAME->actualFPS;
}

JUST_API void frameBufferRender(FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot render in an uninitialized buffer");

  // - - - scan through and only write cells that changed
  for (uint16_t y = 0; y < FRAME->size.height; ++y)
  {
    for (uint16_t x = 0; x < FRAME->size.width; ++x)
    {
      size_t  index     = y * (FRAME->size.width + 1) + x;
      char    backChar  = FRAME->backBuffer[index];
      char    frontChar = FRAME->frontBuffer[index];

      if (backChar != frontChar)
      {
        printf("\033[%d;%dH%c]", FRAME->position.rows + y, FRAME->position.cols + x, backChar);
        FRAME->frontBuffer[index] = backChar;
      }
    }
  }
  fflush(stdout);

  // - - - update timer
  struct timespec currentTime;
  clock_gettime(CLOCK_MONOTONIC, &currentTime);


  uint64_t targetNs  = FRAME->targetFPS > 0 ? 1000000000L / FRAME->targetFPS : 1;
  uint64_t elapsedNs =  (currentTime.tv_sec   - FRAME->lastFrameTime.tv_sec) * 1000000000L +
                        (currentTime.tv_nsec  - FRAME->lastFrameTime.tv_nsec);

  FRAME->actualFPS = 1000000000 / elapsedNs;

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

  static uint8_t frame = 0;
  frame++;
  if (frame % 10 == 0)
  {
    JUST_LOG_TRACE("Actual FPS : %d", FRAME->actualFPS);
  }
}

JUST_API void frameBufferClear(FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot clear an uninitialized buffer");

  for (uint16_t y = 0; y < FRAME->size.height; ++y)
  {
    for (uint16_t x = 0; x < FRAME->size.width; ++x)
    {
      size_t index = y * (FRAME->size.width + 1) + x;
      FRAME->backBuffer[index] = ' ';
    }

    size_t newLineIndex = y * (FRAME->size.width + 1) + FRAME->size.width;
    FRAME->backBuffer[newLineIndex] = '\n';
  }
}

JUST_API void frameBufferSetChar(FrameBuffer* FRAME, TerminalSize POSITION)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot draw in an uninitialized buffer");

  if (POSITION.x >= FRAME->size.width)  return;
  if (POSITION.y >= FRAME->size.height) return;

  size_t index          = POSITION.y * (FRAME->size.width + 1) + POSITION.x;
  FRAME->backBuffer[index]  = '*';
}

JUST_API void frameBufferLine(
    FrameBuffer* FRAME,
    TerminalSize START_POS,
    TerminalSize END_POS)
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
    frameBufferSetChar(FRAME, position);

    error -= dy;
    if (error < 0)
    {
      y     += ystep;
      error += dx;
    }
  }
}

JUST_API void frameBufferTriangle(FrameBuffer* FRAME, TerminalPos POINT_A, TerminalPos POINT_B, TerminalPos POINT_C)
{
  frameBufferLine(FRAME, POINT_A, POINT_B);
  frameBufferLine(FRAME, POINT_B, POINT_C);
  frameBufferLine(FRAME, POINT_C, POINT_A);
}

JUST_API void frameBufferTriangleFilled(FrameBuffer* FRAME, TerminalPos POINT_A, TerminalPos POINT_B, TerminalPos POINT_C)
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
        frameBufferSetChar(FRAME, (TerminalPos){.x = x, .y = y});
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
        frameBufferSetChar(FRAME, (TerminalPos){.x = x, .y = y});
      }
    }
  }
}
