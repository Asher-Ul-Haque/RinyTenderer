#include "rinyTenderer/_structs.h"
#include <bits/time.h>
#include <justLibrary.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static inline TerminalSize resolveActualSize(TerminalSizeRel REL_SIZE)
{
  vec2s windowAbsSize = terminalGetContext()->size;
  float termCols      = windowAbsSize.x;
  float termRows      = windowAbsSize.y;
  
  uint16_t width  = (REL_SIZE.x <= 0.0f) ? (uint16_t)termCols : (uint16_t)(termCols * REL_SIZE.x);
  uint16_t height = (REL_SIZE.y <= 0.0f) ? (uint16_t)termRows : (uint16_t)(termRows * REL_SIZE.y);
  
  return (TerminalSize)
    {
      .x = (float)width, 
      .y = (float)height 
    };
}

static inline TerminalPos resolveActualPos(TerminalPosRel REL_POS)
{
  vec2s windowAbsSize = terminalGetContext()->size;
  return (TerminalPos)
    {
      .x = (float)(uint16_t)(windowAbsSize.x * REL_POS.x), 
      .y = (float)(uint16_t)(windowAbsSize.y * REL_POS.y) 
    };
}


// - - - Create and Destroy - - - 

JUST_API void frameCreateAbsolute(TerminalFrame* FRAME, TerminalPos POSITION, TerminalSize SIZE)
{
  JUST_LOG_DEBUG("[FRAME] : Creating a new frame");
  
  JUST_ASSERT_DEBUG(FRAME != NULL);

  const TerminalCTX* ctx = terminalGetContext();

  if (SIZE.x == 0 || SIZE.y == 0)
  {
    JUST_LOG_WARNING("[FRAME] : Going for maximimum size");
    FRAME->size = ctx->size;
  }
  else
  {
    FRAME->size = SIZE;
  }
  FRAME->position = POSITION;

  size_t width         = FRAME->size.x;
  size_t height        = FRAME->size.y;
  size_t totalCells    = width * height;
  size_t subPixelCells = totalCells * 2;
  
  JUST_LOG_DEBUG(
    "[FRAME] : Size = %u cols x %u rows",
    (unsigned)FRAME->size.y,
    (unsigned)FRAME->size.x);

  // - - - Allocate Buffers
  FRAME->backChars    = (char*)           JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS,        RINY_TENDERER_TAG);
  FRAME->frontChars   = (char*)           JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS,        RINY_TENDERER_TAG);
  FRAME->backFg       = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->frontFg      = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->backBg       = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->frontBg      = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->depthBuffer  = (uint16_t*)       JUST_MALLOC_TAGGED(subPixelCells * sizeof(uint16_t),    RINY_TENDERER_TAG);

  // - - - Check
  JUST_ASSERT_DEBUG(FRAME->backChars    != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontChars   != NULL);
  JUST_ASSERT_DEBUG(FRAME->backFg       != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontFg      != NULL);
  JUST_ASSERT_DEBUG(FRAME->backBg       != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontFg      != NULL);
  JUST_ASSERT_DEBUG(FRAME->depthBuffer  != NULL);

  // - - - refill
  for (size_t i = 0; i < totalCells; ++i)
  {
    char* cell  = &FRAME->backChars[i * CELL_SIZE_CHARS];
    cell[0]     = ' ';
    cell[1]     = '\0';
    cell[2]     = '\0';
    cell[3]     = '\0';
  }
  memset(FRAME->frontChars, 0xFF, totalCells * CELL_SIZE_CHARS);
  memset(FRAME->backFg,     0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->backBg,     0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->frontFg,    0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->frontBg,    0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->depthBuffer,0xFF, subPixelCells * sizeof(uint16_t));

  // - - - setup camera
  cameraInit(&FRAME->camera);
  float aspectRatio = (float) FRAME->size.x / (float) FRAME->size.y;
  cameraSetAspectRatio(&FRAME->camera, aspectRatio);

  // - - - initial ambience light
  FRAME->ambientColor     = (RGBcolor) { 155.0f, 155.0f, 155.0f };
  FRAME->ambientIntensity = 1.0f;

  // - - - initial clear
  frameClear(FRAME, 0);

  JUST_LOG_INFO("[FRAME] : Frame ready");
}

JUST_API void frameCreateRelative(TerminalFrame* FRAME, TerminalPosRel REL_POS, TerminalSizeRel REL_SIZE)
{
  JUST_LOG_DEBUG("[FRAME] : Creating relative frame");
  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot init a NULL FRAME");

  const TerminalCTX* ctx = terminalGetContext();

  vec2s actualSize = resolveActualSize(REL_SIZE);
  vec2s actualPos  = resolveActualPos(REL_POS); 

  frameCreateAbsolute(FRAME, actualPos, actualSize);

  FRAME->widthRatio = (ctx->size.x > 0) ? (float)actualSize.x / (float)ctx->size.x : 1.0f;
  FRAME->relPos     = REL_POS;

  JUST_LOG_INFO("[FRAME] : Relative frame ready (%u x %u)", (unsigned)actualSize.x, (unsigned)actualSize.y);
}

JUST_API void frameDestroy(TerminalFrame* FRAME)
{
  JUST_LOG_DEBUG("[FRAME] : Destroying a FRAME");
  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot destroy a NULL FRAME");

  JUST_FREE(FRAME->backChars);
  JUST_FREE(FRAME->frontChars);
  JUST_FREE(FRAME->backFg);
  JUST_FREE(FRAME->frontFg);
  JUST_FREE(FRAME->backBg);
  JUST_FREE(FRAME->frontBg);
  JUST_FREE(FRAME->depthBuffer);

  FRAME->backChars    = NULL;
  FRAME->frontChars   = NULL;
  FRAME->backFg       = NULL;
  FRAME->backBg       = NULL;
  FRAME->frontFg      = NULL;
  FRAME->frontBg      = NULL;
  FRAME->depthBuffer  = NULL;

  JUST_LOG_INFO("[FRAME] : Destroyed");
}

JUST_API void frameResize(TerminalFrame* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(FRAME->backChars != NULL);

  const TerminalCTX* ctx = terminalGetContext();

  // - - - calculate new sizes 
  uint16_t  newWidth  = (uint16_t) (ctx->size.x * FRAME->widthRatio);
  float     aspect    = (float) FRAME->size.x / (float) FRAME->size.y;
  uint16_t  newHeight = (uint16_t) ((float) newWidth / aspect);

  // - - - maintain a non-zero size
  if (newWidth < 1)  newWidth  = 1;
  if (newHeight < 1) newHeight = 1;

  // - - - calculate new pos
  vec2s newPos = 
  {
    .x = (uint16_t) (ctx->size.x * FRAME->relPos.x),
    .y = (uint16_t) (ctx->size.y * FRAME->relPos.y)
  };

  if (newWidth  == FRAME->size.x      &&
      newHeight == FRAME->size.y      &&
      newPos.x  == FRAME->position.x  &&
      newPos.y  == FRAME->position.y)
  { return; }

  // - - - assign new size and pos
  FRAME->size     = (TerminalSize)
    {
      .x = (float)newWidth, 
      .y = (float)newHeight 
    };
  FRAME->position = newPos;

  // - - - realloc
  size_t totalCells    = (size_t) newWidth * (size_t) newHeight;
  size_t subPixelCells = totalCells * 2;
  
  FRAME->backChars    = (char*)           JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS,        RINY_TENDERER_TAG);
  FRAME->frontChars   = (char*)           JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS,        RINY_TENDERER_TAG);
  FRAME->backFg       = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->frontFg      = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->backBg       = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->frontBg      = (TerminalColor*)  JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor),  RINY_TENDERER_TAG);
  FRAME->depthBuffer  = (uint16_t*)       JUST_MALLOC_TAGGED(subPixelCells * sizeof(uint16_t),    RINY_TENDERER_TAG);

  // - - - Check
  JUST_ASSERT_DEBUG(FRAME->backChars    != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontChars   != NULL);
  JUST_ASSERT_DEBUG(FRAME->backFg       != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontFg      != NULL);
  JUST_ASSERT_DEBUG(FRAME->backBg       != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontFg      != NULL);
  JUST_ASSERT_DEBUG(FRAME->depthBuffer  != NULL);

  // - - - refill
  for (size_t i = 0; i < totalCells; ++i)
  {
    char* cell  = &FRAME->backChars[i * CELL_SIZE_CHARS];
    cell[0]     = ' ';
    cell[1]     = '\0';
    cell[2]     = '\0';
    cell[3]     = '\0';
  }
  memset(FRAME->frontChars, 0xFF, totalCells * CELL_SIZE_CHARS);
  memset(FRAME->backFg,     0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->backBg,     0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->frontFg,    0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->frontBg,    0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->depthBuffer,0xFF, subPixelCells * sizeof(uint16_t));

  JUST_LOG_INFO("[FRAME] : Resized to %u x %u at (%u, %u)",
      (uint16_t)newWidth, (uint16_t)newHeight, (uint16_t)newPos.x, (uint16_t)newPos.y);
}

// - - - Render and clear - - - 

JUST_API void frameClear(TerminalFrame* FRAME, TerminalColor PIXEL_TEMPLATE)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backChars != NULL, "[FRAME] : Cannot clear an uninitialized buffer");

  size_t width         = (size_t)FRAME->size.x;
  size_t height        = (size_t)FRAME->size.y;
  size_t totalCells    = width * height; 
  size_t subPixelCells = totalCells * 2;

  // - - - Reset all characters in the back buffer to spaces (' ')
  for (size_t i = 0; i < totalCells; ++i)
  {
    char* cell  = &FRAME->backChars[i * CELL_SIZE_CHARS];
    cell[0]     = ' ';
    cell[1]     = '\0';
    cell[2]     = '\0';
    cell[3]     = '\0';
    
    // - - - Set background color to the clear template, and foreground to transparent
    FRAME->backBg[i] = PIXEL_TEMPLATE;
    FRAME->backFg[i] = TERMINAL_COLOR_TRANSPARENT;
  }

  // - - - Reset depth buffer back to maximum depth
  memset(FRAME->depthBuffer, 0xFF, subPixelCells * sizeof(uint16_t));
}

// Internal helper for setting cell contents without disturbing depth buffer
static inline void frameInternalSetCell(
    TerminalFrame*  FRAME, 
    int             x, 
    int             y, 
    const char*     CHAR, 
    TerminalColor   FG_COLOR, 
    TerminalColor   BG_COLOR)
{
  size_t index = (size_t)y * FRAME->size.x + (size_t)x;

  char* target = &FRAME->backChars[index * CELL_SIZE_CHARS];
  memset(target, 0, CELL_SIZE_CHARS);
  for (int i = 0; i < CELL_SIZE_CHARS && CHAR[i] != '\0'; ++i)
  { target[i] = CHAR[i]; }

  FRAME->backFg[index] = FG_COLOR;
  if (BG_COLOR != TERMINAL_COLOR_TRANSPARENT)  FRAME->backBg[index] = BG_COLOR;
}

JUST_API void frameSetChar(
    TerminalFrame*  FRAME, 
    FramePos        POSITION, 
    const char*     CHAR, 
    TerminalColor   FG_COLOR, 
    TerminalColor   BG_COLOR)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(FRAME->backChars != NULL);

  int x = (int) POSITION.x;
  int y = (int) POSITION.y / 2.0f;
  int z = (int) POSITION.z;

  if (x < 0)              return;
  if (y < 0)              return;
  if (x >= FRAME->size.x) return;
  if (y >= FRAME->size.y) return;
  
  size_t index = y * FRAME->size.x + x;
  if (z > FRAME->depthBuffer[index * 2]) return;
  FRAME->depthBuffer[index * 2]     = (uint16_t)z;
  FRAME->depthBuffer[index * 2 + 1] = (uint16_t)z;

  frameInternalSetCell(FRAME, x, y, CHAR, FG_COLOR, BG_COLOR);
}

JUST_API void frameSetPixel(
    TerminalFrame*  FRAME, 
    FramePos        POSITION, 
    TerminalColor   COLOR)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(FRAME->backChars != NULL);

  int x     = (int)POSITION.x;
  int subY  = (int)POSITION.y;
  int z     = (int)POSITION.z;

  int   cellY     = subY / 2;
  bool  isBottom  = (subY % 2 != 0);

  if (x < 0)              return;
  if (subY < 0)           return;
  if (x >= FRAME->size.x) return;
  if (subY >= (int)(FRAME->size.y * 2)) return;

  // - - - Sub-pixel independent depth test
  size_t subIndex = (size_t)subY * FRAME->size.x + (size_t)x;
  if (z > FRAME->depthBuffer[subIndex]) return;
  FRAME->depthBuffer[subIndex] = (uint16_t)z;

  size_t index = (size_t)cellY * FRAME->size.x + (size_t)x;

  // - - - Inspect existing cell state
  char*         currentCell = &FRAME->backChars[index * CELL_SIZE_CHARS];
  TerminalColor currentFg   = FRAME->backFg[index];
  TerminalColor currentBg   = FRAME->backBg[index];

  bool hasTop = false;
  bool hasBot = false;

  TerminalColor topColor = 0;
  TerminalColor botColor = 0;

  if (currentCell[0] != '\0' && currentCell[0] != ' ' && currentCell[0] != '\t')
  {
    if (strncmp(currentCell, "▀", 3) == 0)
    {
      hasTop   = true;
      topColor = currentFg;
      if (currentBg != TERMINAL_COLOR_TRANSPARENT)
      {
        hasBot   = true;
        botColor = currentBg;
      }
    }
    else if (strncmp(currentCell, "▄", 3) == 0)
    {
      hasBot   = true;
      botColor = currentFg;
      if (currentBg != TERMINAL_COLOR_TRANSPARENT)
      {
        hasTop   = true;
        topColor = currentBg;
      }
    }
    else
    {
      hasTop   = true;
      hasBot   = true;
      topColor = currentFg;
      botColor = currentFg;
    }
  }

  // - - - Apply the new pixel to the correct half
  if (isBottom)
  {
    hasBot   = true;
    botColor = COLOR;
  }
  else
  {
    hasTop   = true;
    topColor = COLOR;
  }

  // - - - Determine character and colors (preserve frame background for empty halves)
  const char*   targetChar  = " ";
  TerminalColor finalFg     = TERMINAL_COLOR_TRANSPARENT;
  TerminalColor finalBg     = currentBg;

  if (hasTop && hasBot)
  {
    targetChar  = "▀";
    finalFg     = topColor;
    finalBg     = botColor;
  }
  else if (hasTop && !hasBot)
  {
    targetChar  = "▀";
    finalFg     = topColor;
    finalBg     = (currentBg != TERMINAL_COLOR_TRANSPARENT) ? currentBg : 0;
  }
  else if (!hasTop && hasBot)
  {
    targetChar  = "▄";
    finalFg     = botColor;
    finalBg     = (currentBg != TERMINAL_COLOR_TRANSPARENT) ? currentBg : 0;
  }

  // Update cell buffers directly without re-testing/overwriting depth buffer
  frameInternalSetCell(FRAME, x, cellY, targetChar, finalFg, finalBg);
}

JUST_API void frameRender(TerminalFrame* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(FRAME->backChars != NULL);

  static char scratch[UINT16_MAX];
  int offset = 0;
  int lastFg = -1;
  int lastBg = -2;

  size_t width  = (size_t)FRAME->size.x;
  size_t height = (size_t)FRAME->size.y;

  int frameScreenX = (int)FRAME->position.x;
  int frameScreenY = (int)FRAME->position.y;

  for (size_t y = 0; y < height; ++y)
  {
    for (size_t x = 0; x < width; ++x)
    {
      size_t index = y * width + x;

      char* currChar  = &FRAME->backChars[index * CELL_SIZE_CHARS];
      char* frontChar = &FRAME->frontChars[index * CELL_SIZE_CHARS];

      TerminalColor currFg  = FRAME->backFg[index];
      TerminalColor currBg  = FRAME->backBg[index];
      TerminalColor frontFg = FRAME->frontFg[index];
      TerminalColor frontBg = FRAME->frontBg[index];

      // - - - Check if cell content or colors have changed
      bool charChanged = (memcmp(currChar, frontChar, CELL_SIZE_CHARS) != 0);
      bool fgChanged   = (currFg != frontFg);
      bool bgChanged   = (currBg != frontBg);

      if (charChanged || fgChanged || bgChanged)
      {
        // - - - Move cursor to absolute terminal coordinate
        int termRow = frameScreenY + (int)y + 1;
        int termCol = frameScreenX + (int)x + 1;
        offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[%d;%dH", termRow, termCol);

        // - - - Update Foreground color
        if ((int)currFg != lastFg)
        {
          offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[38;5;%dm", (int)currFg);
          lastFg = (int)currFg;
        }

        // - - - Update Background color
        if (currBg == TERMINAL_COLOR_TRANSPARENT)
        {
          if (lastBg != -1)
          {
            offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[49m");
            lastBg = -1;
          }
        }
        else
        {
          if ((int)currBg != lastBg)
          {
            offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[48;5;%dm", (int)currBg);
            lastBg = (int)currBg;
          }
        }

        // - - - Print character
        const char* printChar = (currChar[0] != '\0') ? currChar : " ";
        offset += snprintf(scratch + offset, sizeof(scratch) - offset, "%.4s", printChar);

        // - - - Sync front buffers
        memcpy(frontChar, currChar, CELL_SIZE_CHARS);
        FRAME->frontFg[index] = currFg;
        FRAME->frontBg[index] = currBg;
      }
    }
  }

  // Reset formatting attributes and flush output in a single write call
  if (offset > 0)
  {
      offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[0m");
      fwrite(scratch, 1, (size_t)offset, stdout);
      fflush(stdout);
  }
}
