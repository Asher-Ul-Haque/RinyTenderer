#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

JUST_API void tuiDrawProgressBar(TerminalFrame* FRAME, TuiProgressBar PROG_BAR_DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  float prog = PROG_BAR_DESC.progress;
  if (prog > 1.0f) prog = 1.0f;
  if (prog < 0.0f) prog = 0.0f;

  uint16_t length      = PROG_BAR_DESC.length;
  uint16_t filledUnits = (uint16_t)((float)length * prog);

  bool isHorizontal = (PROG_BAR_DESC.orientation == TUI_HORIZONTAL);

  // Snap Y to the nearest even sub-pixel cell row boundary
  int cellY = (int)PROG_BAR_DESC.position.y / 2;
  float startY = (float)(cellY * 2);

  // - - - Draw bar segments using full cell characters (always wide/solid)
  for (uint16_t i = 0; i < length; ++i)
  {
    TerminalColor col = (i < filledUnits) ? PROG_BAR_DESC.fillColor : PROG_BAR_DESC.emptyColor;
    FramePos cellPos = 
      {
        .x = PROG_BAR_DESC.position.x + (isHorizontal ? (float)i : 0.0f),
        .y = startY + (isHorizontal ? 0.0f : (float)(length - 1 - i) * 2.0f),
        .z = PROG_BAR_DESC.position.z
      };
    // Draw solid cell block
    frameSetChar(FRAME, cellPos, " ", col, col);
  }

  // - - - Percentage Overlay (Active only when showPercentage is true)
  if (PROG_BAR_DESC.showPercentage && length >= 5)
  {
    char  pctStr[8];
    int   pct = (int)(prog * 100.0f + 0.5f);
    snprintf(pctStr, sizeof(pctStr), "%d%%", pct);
    size_t len = strlen(pctStr);

    if (length >= len)
    {
      FramePos textPos;
      if (isHorizontal)
      {
        uint16_t startOffset = (length - (uint16_t)len) / 2;
        textPos = (FramePos)
          {
            .x = PROG_BAR_DESC.position.x + (float)startOffset,
            .y = startY,
            .z = PROG_BAR_DESC.position.z - 10.0f
          };
      }
      else
      {
        uint16_t startOffset = (length >= len) ? (length - (uint16_t)len) / 2 : 0;
        textPos = (FramePos)
          {
            .x = PROG_BAR_DESC.position.x,
            .y = startY + (float)(startOffset * 2),
            .z = PROG_BAR_DESC.position.z - 10.0f
          };
      }

      tuiDrawText(FRAME, (TuiText)
      {
        .position = textPos,
        .text     = pctStr,
        .maxLen   = len,
        .width    = (uint16_t)len,
        .fgColor  = PROG_BAR_DESC.textColor,
        .bgColor  = TERMINAL_COLOR_TRANSPARENT
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

  // - - - Snap Y to the nearest even sub-pixel cell row boundary
  int cellY = (int)DESC.position.y / 2;
  FramePos topLeft = 
    {
      .x = DESC.position.x,
      .y = (float)(cellY * 2),
      .z = DESC.position.z
    };

  // - - - Compute dynamic bounding box
  float    trackLength   = (float)DESC.length;
  FramePos bottomRight    =
    {
      .x = (DESC.orientation == TUI_HORIZONTAL) 
            ? topLeft.x + trackLength 
            : topLeft.x + 1.0f,
      .y = (DESC.orientation == TUI_HORIZONTAL) 
            ? topLeft.y + 1.0f 
            : topLeft.y + (float)(DESC.length * 2) - 1.0f,
      .z = topLeft.z
    };

  // - - - Mouse Hit-Testing & Focus Tracking
  static const float* focusedSlider = NULL;
  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT))
  {
    TerminalPos mousePos      = terminalGetMousePos();
    FramePos    mouseFramePos = tuiMapMouseToFrame(FRAME, mousePos);

    if (tuiIsPointInBounds(mouseFramePos, topLeft, bottomRight))
    {
      focusedSlider = DESC.value;
      
      float relPos  = (DESC.orientation == TUI_HORIZONTAL) 
                    ? (mouseFramePos.x - topLeft.x) 
                    : (mouseFramePos.y - topLeft.y) / 2.0f;

      if (relPos < 0.0f)        relPos = 0.0f;
      if (relPos > trackLength) relPos = trackLength;

      float ratio = (trackLength > 0.0f) 
                    ? (relPos / trackLength) 
                    : 0.0f;
      *(DESC.value) = DESC.minValue + ratio * (DESC.maxValue - DESC.minValue);
    }
  }

  // - - - Arrow Key Navigation when Focused
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

  // - - - Clamp Value Bounds
  if (*(DESC.value) < DESC.minValue) *(DESC.value) = DESC.minValue;
  if (*(DESC.value) > DESC.maxValue) *(DESC.value) = DESC.maxValue;

  // - - - Render Slider Track via tuiDrawProgressBar
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

  // - - - Draw Label and Value using tuiDrawText centered over the track
  if (DESC.showValue && DESC.label != NULL && DESC.maxLen > 0)
  {
    char    displayStr[64];
    int     written = snprintf(displayStr, sizeof(displayStr), "%.*s: %.1f", (int)DESC.maxLen, DESC.label, *(DESC.value));
    size_t  strLen  = (written > 0) ? (size_t)written : 0;

    float centerX = topLeft.x + (trackLength / 2.0f) - ((float)strLen / 2.0f);
    float centerY = topLeft.y;

    tuiDrawText(FRAME, (TuiText)
    {
      .position = 
        {
          .x = centerX,
          .y = centerY,
          .z = topLeft.z - 10.0f
        },
      .text     = displayStr,
      .maxLen   = strLen,
      .width    = (uint16_t)strLen,
      .fgColor  = DESC.accentColor,
      .bgColor  = TERMINAL_COLOR_TRANSPARENT
    });
  }
}
