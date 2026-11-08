#include "justLibrary.h"
#include "rinyTenderer/_structs.h"
#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>
#include <stdint.h>

JUST_API void tuiDrawProgressBar(TerminalFrame* FRAME, TuiProgressBar PROG_BAR_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  float prog = PROG_BAR_DESC.progress;
  if (prog > 1.0f) prog = 1.0f;
  if (prog < 0.0f) prog = 0.0f;

  uint16_t length       = PROG_BAR_DESC.length;
  uint16_t filledUnits = (uint16_t)((float)length * prog);

  bool isHorizontal = (PROG_BAR_DESC.orientation == TUI_HORIZONTAL);
  bool isThick      = PROG_BAR_DESC.showPercentage;

  // - - - Draw bar segments
  for (uint16_t i = 0; i < length; ++i)
  {
    TerminalColor col = (i < filledUnits) ? PROG_BAR_DESC.fillColor : PROG_BAR_DESC.emptyColor;

    FramePos cellPos = 
      {
        .x = PROG_BAR_DESC.position.x + (isHorizontal ? i : 0),
        .y = PROG_BAR_DESC.position.y + (isHorizontal ? 0 : (length - 1 - i)),
        .z = PROG_BAR_DESC.position.z
      };

    frameSetPixel(FRAME, cellPos, col);

    if (isThick)
    {
      FramePos extra = cellPos;
      if (isHorizontal)
      {
        extra.y += 1.0f;
      }
      else
      {
        extra.x += 1.0f;
      }
      frameSetPixel(FRAME, extra, col);
    }
  }

  // - - - Percentage Overlay (Active whenever thick is true)
  if (isThick && length >= 5)
  {
    char pctStr[8];
    int pct = (int)(prog * 100.0f + 0.5f);
    snprintf(pctStr, sizeof(pctStr), "%d", pct);
    size_t len = strlen(pctStr);

    if (length >= len)
    {
      FramePos textPos;
      if (isHorizontal)
      {
        uint16_t startOffset = (length - (uint16_t)len) / 2;
        textPos = (FramePos){
          .x = PROG_BAR_DESC.position.x + startOffset,
          .y = PROG_BAR_DESC.position.y,
          .z = PROG_BAR_DESC.position.z + 10.0f
        };
      }
      else
      {
        // Center vertically along the vertical bar
        uint16_t startOffset = (length >= len) ? (length - (uint16_t)len) / 2 : 0;
        textPos = (FramePos){
          .x = PROG_BAR_DESC.position.x,
          .y = PROG_BAR_DESC.position.y + (startOffset * 2), // Map to cell row
          .z = PROG_BAR_DESC.position.z + 10.0f
        };
      }

      tuiDrawText(FRAME, (TuiText){
        .position = textPos,
        .text = pctStr,
        .maxLen = len,
        .fgColor = PROG_BAR_DESC.textColor,
        .bgColor = TERMINAL_COLOR_TRANSPARENT
      });
    }
  }
}
