#pragma once

#include <rinyTenderer/window.h>
#include <rinyTenderer/_structs.h>
#include <justLibrary.h>

JUST_API void tuiInit(void);

JUST_API void tuiDestroy(void);

JUST_API void tuiResize(void);

JUST_API void tuiClear(void);

JUST_API void tuiRender(void);

JUST_API void tuiSetChar(TerminalPos POSITION, const char* CHAR, TerminalColor FG_COLOR, TerminalColor BG_COLOR);

JUST_API void tuiAddRect(
    TerminalPos   TOP_LEFT, 
    TerminalPos   BOTTOM_RIGHT, 
    TerminalColor COLOR,
    bool          ROUNDED);
