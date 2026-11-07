#pragma once

#include <rinyTenderer/window.h>
#include <rinyTenderer/_structs.h>
#include <justLibrary.h>


// - - - TUI Box & Panel Drawing - - -

typedef enum tuiBoxType
{
  ROUNDED,
  SHARP,
  HEAVY,
  DOUBLE,
  ASCII
} TuiBoxType;

JUST_API void tuiDrawBox(
    TerminalFrame*  FRAME, 
    FramePos        TOP_LEFT,
    FramePos        BOTTOM_RIGHT,
    TerminalColor   COLOR,
    TuiBoxType      BOX_TYPE);

JUST_API void tuiFillRect(
    TerminalFrame*  FRAME,
    FramePos        TOP_LEFT,
    FramePos        BOTTOM_RIGHT,
    const char*     CHARACTER,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR);


// - - - TUI Text & Label Rendering - - -

JUST_API void tuiDrawText(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    const char*     TEXT,
    size_t          TEXT_LENGTH,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR);

JUST_API void tuiDrawTextCentered(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    uint16_t        WIDTH,
    const char*     TEXT,
    size_t          TEXT_LENGTH,
    TerminalColor   FG_COLOR,
    TerminalColor   BG_COLOR);


// - - - TUI Widgets & Meters - - -

JUST_API void tuiDrawProgressBar(
    TerminalFrame*  FRAME,
    FramePos        POSITION,
    uint16_t        WIDTH,
    float           PROGRESS,
    TerminalColor   FILL_COLOR,
    TerminalColor   EMPTY_COLOR);
