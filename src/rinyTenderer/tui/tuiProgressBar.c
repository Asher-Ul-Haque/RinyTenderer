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

JUST_API void tuiSlider(TerminalFrame* FRAME, const TuiSlider DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(DESC.value != NULL);
  JUST_ASSERT_DEBUG(DESC.minValue < DESC.maxValue);
  JUST_ASSERT_DEBUG(DESC.length > 0);

  // - - - 1. Compute dynamic bounding box
  FramePos topLeft = DESC.position;
  FramePos bottomRight = {
    .x = (DESC.orientation == TUI_HORIZONTAL) ? topLeft.x + (float)DESC.length : topLeft.x + 4.0f,
    .y = (DESC.orientation == TUI_HORIZONTAL) ? topLeft.y + 2.0f : topLeft.y + (float)(DESC.length * 2),
    .z = topLeft.z
  };

  float trackLength = (float)DESC.length;

  // - - - 2. Mouse Hit-Testing & Focus Tracking
  static const float* focusedSlider = NULL;

  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT))
  {
    TerminalPos mousePos = terminalGetMousePos();
    FramePos mouseFramePos = {
      .x = (float)mousePos.x,
      .y = (float)mousePos.y * 2.0f - 2.0f,
      .z = 0.0f
    };

    if (tuiIsPointInBounds(mouseFramePos, topLeft, bottomRight))
    {
      focusedSlider = DESC.value;
      float relPos = (DESC.orientation == TUI_HORIZONTAL) ? (mouseFramePos.x - topLeft.x) : (mouseFramePos.y - topLeft.y);
      if (relPos < 0.0f) relPos = 0.0f;
      if (relPos > trackLength) relPos = trackLength;

      float ratio = (trackLength > 0.0f) ? (relPos / trackLength) : 0.0f;
      *(DESC.value) = DESC.minValue + ratio * (DESC.maxValue - DESC.minValue);
    }
  }

  // - - - 3. Arrow Key Navigation when Focused
  if (focusedSlider == DESC.value)
  {
    float step = (DESC.maxValue - DESC.minValue) * 0.05f;
    if (terminalIsKeyDown(KEY_LEFT) || terminalIsKeyDown(KEY_DOWN))
    {
      *(DESC.value) -= step;
    }
    if (terminalIsKeyDown(KEY_RIGHT) || terminalIsKeyDown(KEY_UP))
    {
      *(DESC.value) += step;
    }
  }

  // - - - 4. Clamp Value Bounds
  if (*(DESC.value) < DESC.minValue) *(DESC.value) = DESC.minValue;
  if (*(DESC.value) > DESC.maxValue) *(DESC.value) = DESC.maxValue;

  // - - - 5. Render Progress Bar
  float currentRatio = (*(DESC.value) - DESC.minValue) / (DESC.maxValue - DESC.minValue);

  tuiDrawProgressBar(FRAME, (TuiProgressBar)
  {
    .fillColor      = DESC.fillColor,
    .emptyColor     = DESC.emptyColor,
    .textColor      = DESC.accentColor,
    .position       = topLeft,
    .orientation    = DESC.orientation,
    .length         = DESC.length,
    .progress       = currentRatio,
    .showPercentage = false
  });

  // - - - 6. Draw Label and Value if Enabled
  if (DESC.showValue && DESC.label != NULL && DESC.maxLen > 0)
  {
    char displayStr[64];
    int written = snprintf(displayStr, sizeof(displayStr), "%.*s: %.1f", (int)DESC.maxLen, DESC.label, *(DESC.value));
    size_t strLen = (written > 0) ? (size_t)written : 0;

    float centerX = topLeft.x + (trackLength / 2.0f) - ((float)strLen / 2.0f);
    float centerY = topLeft.y + 1.0f;

    for (size_t i = 0; i < strLen; ++i)
    {
      char symbol[2] = { displayStr[i], '\0' };
      FramePos charPos = {
        .x = centerX + (float)i,
        .y = centerY,
        .z = topLeft.z - 2.0f
      };
      frameSetChar(FRAME, charPos, symbol, DESC.accentColor, TERMINAL_COLOR_TRANSPARENT);
    }
  }
}
