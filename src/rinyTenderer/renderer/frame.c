#include <justLibrary.h>
#include <math.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stdint.h>
#include <string.h>
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

  FRAME->buffer = (char*) JUST_MALLOC_TAGGED(totalSize, "TERM");
  JUST_ASSERT_MESSAGE(FRAME->buffer != NULL, "[FRAME] : Allocation for this frame failed");


  // - - - Populate buffer with spaces, newlines at row ends and \0 at the end
  for (uint16_t r = 0; r < height; ++r)
  {
    size_t rowOffset = r * (width + 1);
    for (uint16_t c = 0; c < width; ++c)
    {
      FRAME->buffer[rowOffset + c] = ' ';
    }
    FRAME->buffer[rowOffset + width] = '\n';
  }
  FRAME->buffer[totalSize - 1] = '\0';

  JUST_LOG_INFO("[FRAME] : Frame ready");
}

JUST_API void frameBufferDestroy(FrameBuffer* FRAME)
{
  JUST_LOG_DEBUG("[FRAME] : Destroying a FRAME");

  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot destroy a NULL FRAME");

  JUST_FREE(FRAME->buffer);
  FRAME->buffer = NULL;

  JUST_LOG_INFO("[FRAME] : Destroyed");
}

JUST_API void frameBufferRender(const FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  size_t width      = FRAME->size.width;
  size_t height     = FRAME->size.height;
  size_t totalSize  = (width + 1) * height;

  terminalSetCursor(FRAME->position);
  fflush(stdout);
  write(STDOUT_FILENO, FRAME->buffer, totalSize);
}

JUST_API void frameBufferSetChar(FrameBuffer* FRAME, TerminalSize POSITION)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->buffer != NULL, "[FRAME] : Cannot draw in an uninitialized buffer");

  if (POSITION.x >= FRAME->size.width)  return;
  if (POSITION.y >= FRAME->size.height) return;

  size_t index          = POSITION.y * (FRAME->size.width + 1) + POSITION.x;
  FRAME->buffer[index]  = '*';
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
