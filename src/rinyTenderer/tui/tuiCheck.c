#include <rinyTenderer/renderer/frame.h>
#include <rinyTenderer/tui/tui.h>

JUST_API bool tuiCheckbox(TerminalFrame* FRAME, const TuiCheckbox DESC)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(DESC.label != NULL);
  JUST_ASSERT_DEBUG(DESC.checked != NULL);

  size_t  labelLen    = strlen(DESC.label);
  float   totalWidth  = 2.0f + (float)labelLen;

  FramePos topLeft = DESC.position;
  FramePos bottomRight = 
    {
      .x = topLeft.x + totalWidth,
      .y = topLeft.y + 2.0f,
      .z = topLeft.z
    };

  // - - - 1. Hit-Test mouse click or Enter key
  bool toggled = false;
  if (terminalIsMouseButtonDown(MOUSE_BUTTON_LEFT) || terminalIsKeyDown(KEY_ENTER))
  {
    TerminalPos mousePos      = terminalGetMousePos();
    FramePos    mouseFramePos = 
      {
        .x = (float)mousePos.x,
        .y = (float)mousePos.y * 2.0f - 2.0f,
        .z = 0.0f
      };

    if (tuiIsPointInBounds(mouseFramePos, topLeft, bottomRight))
    {
      *(DESC.checked) = !(*(DESC.checked));
      toggled         = true;
    }
  }

  // - - - 2. Draw Ballot Box character
  const char* boxChar = *(DESC.checked) ? "☑" : "☐";
  frameSetChar(FRAME, topLeft, boxChar, DESC.boxColor, TERMINAL_COLOR_TRANSPARENT);

  // - - - 3. Draw Label Text next to the box
  for (size_t i = 0; i < labelLen; ++i)
  {
    char symbol[2]    = { DESC.label[i], '\0' };
    FramePos charPos  = 
      {
        .x = topLeft.x + 2.0f + (float)i,
        .y = topLeft.y,
        .z = topLeft.z
      };
    frameSetChar(FRAME, charPos, symbol, DESC.textColor, TERMINAL_COLOR_TRANSPARENT);
  }

  return toggled;
}
