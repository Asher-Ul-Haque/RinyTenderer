#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include "rinyTenderer/window.h"
#include <rinyTenderer/tui/tui.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct tuiContext
{
  uint16_t        width;
  uint16_t        height;
  char*           cells; 
  char*           frontCells;
  TerminalColor*  fgColors;
  TerminalColor*  fgFront;
  TerminalColor*  bgColors;
  TerminalColor*  bgFront;
} TuiContext;

static TuiContext ctx;

#define CELL_SIZE_CHARS 4


// - - - Life cycle - - - 

JUST_API void tuiInit(void)
{
  JUST_ASSERT_DEBUG(ctx.cells == NULL);

  // - - - get data
  const TerminalCTX* termCTX  = terminalGetContext();
  ctx.width                   = termCTX->size.x;
  ctx.height                  = termCTX->size.y / 2;
  size_t totalCells           = (size_t) ctx.width * ctx.height;

  // - - - Malloc data 
  ctx.cells       = (char*) JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS, RINY_TENDERER_TAG);
  ctx.frontCells  = (char*) JUST_MALLOC_TAGGED(totalCells * CELL_SIZE_CHARS, RINY_TENDERER_TAG);
  ctx.fgColors    = (uint8_t*) JUST_MALLOC_TAGGED(totalCells, RINY_TENDERER_TAG);
  ctx.fgFront     = (uint8_t*) JUST_MALLOC_TAGGED(totalCells, RINY_TENDERER_TAG);
  ctx.bgColors    = (uint8_t*) JUST_MALLOC_TAGGED(totalCells, RINY_TENDERER_TAG);
  ctx.bgFront     = (uint8_t*) JUST_MALLOC_TAGGED(totalCells, RINY_TENDERER_TAG);

  // - - - sanity check 
  JUST_ASSERT_DEBUG_MESSAGE(ctx.cells       != NULL, "[TUI] : Allocation failed");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.frontCells  != NULL, "[TUI] : Allocation failed");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.fgColors    != NULL, "[TUI] : Allocation failed");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.fgFront     != NULL, "[TUI] : Allocation failed");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.bgColors    != NULL, "[TUI] : Allocation failed");
  JUST_ASSERT_DEBUG_MESSAGE(ctx.bgFront     != NULL, "[TUI] : Allocation failed");


  // - - - reset 
  tuiClear();
  memset(ctx.frontCells, 0xFF, totalCells * CELL_SIZE_CHARS);
}

JUST_API void tuiDestroy(void)
{
  JUST_ASSERT_DEBUG(ctx.cells != NULL);

  JUST_FREE(ctx.cells);
  JUST_FREE(ctx.frontCells);
  JUST_FREE(ctx.fgColors);
  JUST_FREE(ctx.fgFront);
  JUST_FREE(ctx.bgColors);
  JUST_FREE(ctx.bgFront);

  ctx.cells       = NULL;
  ctx.frontCells  = NULL;
  ctx.fgColors    = NULL;
  ctx.fgFront     = NULL;
  ctx.bgColors    = NULL;
  ctx.bgFront     = NULL;

  memset(&ctx, 0, sizeof(ctx));
}

JUST_API void tuiResize(void)
{
  const TerminalCTX*  termCTX   = terminalGetContext();
  uint16_t            newWidth  = (uint16_t) termCTX->size.x;
  uint16_t            newHeight = (uint16_t) termCTX->size.y / 2;

  if (newWidth == ctx.width && newHeight == ctx.height) return;

  tuiDestroy();
  tuiInit();
}

JUST_API void tuiClear(void)
{
  size_t totalCells = (size_t) ctx.width * ctx.height;

  for (size_t i = 0; i < totalCells; ++i)
  {
    char* cell = &ctx.cells[i * 4];
    cell[0] = ' ';
    cell[1] = '\0';
    cell[2] = '\0';
    cell[3] = '\0';
  
    ctx.fgColors[i] = TERMINAL_COLOR_TRANSPARENT;
    ctx.bgColors[i] = TERMINAL_COLOR_TRANSPARENT;
  }
}

JUST_API void tuiRender(void)
{
    JUST_ASSERT_DEBUG(ctx.cells != NULL);

    static char scratch[UINT16_MAX];
    int offset = 0;
    int lastFg = -1;
    int lastBg = -2;

    for (uint16_t y = 0; y < ctx.height; ++y)
    {
        for (uint16_t x = 0; x < ctx.width; ++x)
        {
            size_t index = (size_t)y * ctx.width + x;

            char* currChar  = &ctx.cells[index * CELL_SIZE_CHARS];
            char* frontChar = &ctx.frontCells[index * CELL_SIZE_CHARS];

            TerminalColor currFg  = ctx.fgColors[index];
            TerminalColor currBg  = ctx.bgColors[index];
            TerminalColor frontFg = ctx.fgFront[index];
            TerminalColor frontBg = ctx.bgFront[index];

            // Check if cell has changed compared to front buffer
            bool charChanged = (memcmp(currChar, frontChar, CELL_SIZE_CHARS) != 0);
            bool fgChanged   = (currFg != frontFg);
            bool bgChanged   = (currBg != frontBg);

            if (charChanged || fgChanged || bgChanged)
            {
                // If the cell is completely transparent and empty, we want to "clear" it 
                // back to transparent if it *used* to have something there, otherwise skip it.
                bool isTransparentEmpty = (currBg == TERMINAL_COLOR_TRANSPARENT && (currChar[0] == '\0' || currChar[0] == ' '));

                // If it's transparent and empty, and the front buffer was also empty, skip entirely
                if (isTransparentEmpty && (frontChar[0] == '\0' || frontChar[0] == ' ') && frontBg == TERMINAL_COLOR_TRANSPARENT)
                {
                    // Sync front buffer and skip
                    memcpy(frontChar, currChar, CELL_SIZE_CHARS);
                    ctx.fgFront[index] = currFg;
                    ctx.bgFront[index] = currBg;
                    continue;
                }

                // 1. Explicitly jump to this cell's absolute coordinate (1-indexed)
                offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[%d;%dH", y + 1, x + 1);

                // 2. Update Foreground color if changed
                if ((int)currFg != lastFg)
                {
                    offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[38;5;%dm", (int)currFg);
                    lastFg = (int)currFg;
                }

                // 3. Update Background color if changed
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

                // 4. Print character
                const char* printChar = (currChar[0] != '\0') ? currChar : " ";
                offset += snprintf(scratch + offset, sizeof(scratch) - offset, "%.4s", printChar);

                // 5. Sync front buffer
                memcpy(frontChar, currChar, CELL_SIZE_CHARS);
                ctx.fgFront[index] = currFg;
                ctx.bgFront[index] = currBg;
            }
        }
    }

    // Reset styles at the end of the frame draw batch
    if (offset > 0)
    {
        offset += snprintf(scratch + offset, sizeof(scratch) - offset, "\033[0m");
        fwrite(scratch, 1, offset, stdout);
        fflush(stdout);
    }
}

JUST_API void tuiSetChar(
    TerminalPos     POSITION, 
    const char*     CHAR, 
    TerminalColor FG_COLOR, 
    TerminalColor BG_COLOR)
{
    JUST_ASSERT_DEBUG(ctx.cells != NULL);

    uint16_t x = POSITION.x;
    uint16_t y = POSITION.y / 2;

    if (x >= ctx.width || y >= ctx.height) return;

    size_t index = (size_t) y * ctx.width + x;
    char* target = &ctx.cells[index * CELL_SIZE_CHARS];

    memset(target, 0, CELL_SIZE_CHARS);
    for (int i = 0; i < CELL_SIZE_CHARS && CHAR[i] != '\0'; ++i)
    {
        target[i] = CHAR[i];
    }

    ctx.fgColors[index] = FG_COLOR;
    ctx.bgColors[index] = BG_COLOR;
}

JUST_API void tuiAddRect(
    TerminalPos     TOP_LEFT, 
    TerminalPos     BOTTOM_RIGHT, 
    TerminalColor COLOR, 
    bool            ROUNDED)
{
    JUST_ASSERT_DEBUG(ctx.cells != NULL);

    // Keep coordinates in pixel/half-block space; tuiSetChar handles the /2 division
    uint16_t x1 = TOP_LEFT.x;
    uint16_t y1 = TOP_LEFT.y;
    uint16_t x2 = BOTTOM_RIGHT.x;
    uint16_t y2 = BOTTOM_RIGHT.y;

    if (x1 > x2 || y1 > y2) return;

    const char* tl = ROUNDED ? "╭" : "┌";
    const char* tr = ROUNDED ? "╮" : "┐";
    const char* bl = ROUNDED ? "╰" : "└";
    const char* br = ROUNDED ? "╯" : "┘";
    const char* h  = "─";
    const char* v  = "│";

    // Corners (pixel coordinates)
    tuiSetChar((TerminalPos){.x = x1, .y = y1}, tl, COLOR, TERMINAL_COLOR_TRANSPARENT);
    tuiSetChar((TerminalPos){.x = x2, .y = y1}, tr, COLOR, TERMINAL_COLOR_TRANSPARENT);
    tuiSetChar((TerminalPos){.x = x1, .y = y2}, bl, COLOR, TERMINAL_COLOR_TRANSPARENT);
    tuiSetChar((TerminalPos){.x = x2, .y = y2}, br, COLOR, TERMINAL_COLOR_TRANSPARENT);

    // Horizontal edges (step by 1 x-pixel)
    for (uint16_t x = x1 + 1; x < x2; ++x)
    {
        tuiSetChar((TerminalPos){.x = x, .y = y1}, h, COLOR, TERMINAL_COLOR_TRANSPARENT);
        tuiSetChar((TerminalPos){.x = x, .y = y2}, h, COLOR, TERMINAL_COLOR_TRANSPARENT);
    }

    // Vertical edges (step by 2 y-pixels to match text rows)
    for (uint16_t y = y1 + 2; y < y2; y += 2)
    {
        tuiSetChar((TerminalPos){.x = x1, .y = y}, v, COLOR, TERMINAL_COLOR_TRANSPARENT);
        tuiSetChar((TerminalPos){.x = x2, .y = y}, v, COLOR, TERMINAL_COLOR_TRANSPARENT);
    }
}
