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

JUST_API void frameBufferLine(FrameBuffer* FRAME, TerminalSize START_POS, TerminalSize END_POS)
{

  for (float t = 0; t < 1; t += 0.01f)
  {
    uint16_t x = round(START_POS.x + (END_POS.x - START_POS.x) * t);
    uint16_t y = round(START_POS.y + (END_POS.y - START_POS.y) * t); 
    frameBufferSetChar(FRAME, (TerminalSize) { .x = x, .y = y });
  }
}
