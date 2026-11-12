#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>

#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>
#include <string.h>

#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>
#include <string.h>

JUST_API bool tuiCheckbox(TerminalFrame* FRAME, const TuiCheckbox DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(DESC.label != NULL);
  JUST_ASSERT_DEBUG(DESC.checked != NULL);

  size_t labelLen = strlen(DESC.label);

  // - - - Align topLeft.y with frameSetChar's internal cell row calculation
  int cellY = (int)DESC.position.y / 2;
  FramePos topLeft = 
    {
      .x = DESC.position.x,
      .y = (float)(cellY * 2),
      .z = DESC.position.z
    };

  // - - - Hit-Test mouse click or Enter key against the exact ballot box cell
  bool toggled = false;
  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT) || terminalIsKeyDown(KEY_ENTER))
  {
    TerminalPos mousePos      = terminalGetMousePos();
    FramePos    mouseFramePos = tuiMapMouseToFrame(FRAME, mousePos);

    if (topLeft.x == mouseFramePos.x && topLeft.y == mouseFramePos.y)
    {
      *(DESC.checked) = !(*(DESC.checked));
      toggled         = true;
    }
  }

  // - - - Draw Ballot Box character
  const char* boxChar = *(DESC.checked) ? "☒" : "☐";
  frameSetChar(FRAME, topLeft, boxChar, DESC.boxColor, TERMINAL_COLOR_TRANSPARENT);

  // - - - Draw Label Text next to the box using tuiDrawText
  tuiDrawText(FRAME, (TuiText)
  {
    .position = 
      {
        .x = topLeft.x + 2.0f,
        .y = topLeft.y,
        .z = topLeft.z
      },
    .text     = DESC.label,
    .maxLen   = labelLen,
    .width    = (uint16_t)labelLen,
    .fgColor  = DESC.textColor,
    .bgColor  = TERMINAL_COLOR_TRANSPARENT
  });

  return toggled;
}
