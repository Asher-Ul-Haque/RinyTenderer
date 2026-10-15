/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies
 */

#pragma once
#include "justLibrary.h"
#include <rinyTenderer/window.h>
#include <stdint.h>

/// @brief : Say a frame in a window, a window can have multiple frames
typedef struct framebuffer
{
  TerminalSize    position;       ///< Where in the terminal
  TerminalSize    size;           ///< How big is the terminal
  char*           backBuffer;     ///< Where to draw 
  char*           frontBuffer;    ///< What to show
  struct timespec lastFrameTime;  ///< Last frame stat
  uint8_t         targetFPS;      ///< duh
  uint8_t         actualFPS;      ///< duh
} FrameBuffer;

/// @brief : Pass this for the frame to occupy all the window
#define FULLSCREEN_FRAME_SIZE (TerminalSize) { .rows = 0, .cols = 0 }

/// @brief : Pass this for the frame to be at the top left of the window 
#define TOP_LEFT_FRAME_POS (TerminalPos) { .rows = 1, .cols = 1 } 

/// @brief : No limits on fps, I paid for the entire CPU
#define UNCAPPED_FRAME_FPS 0


JUST_API void frameBufferInit(FrameBuffer* FRAME, TerminalPos POSITION, TerminalSize SIZE);

JUST_API void frameBufferDestroy(FrameBuffer* FRAME);

JUST_API void frameBufferSetFPS(FrameBuffer* FRAME, uint8_t TARGET_FPS);

JUST_API uint8_t frameBufferGetFPS(const FrameBuffer* FRAME);

JUST_API void frameBufferRender(FrameBuffer* FRAME);

JUST_API void frameBufferClear(FrameBuffer* FRAME);

JUST_API void frameBufferSetChar(FrameBuffer* FRAME, TerminalPos POSITION);

JUST_API void frameBufferLine(FrameBuffer* FRAME, TerminalPos START_POS, TerminalPos END_POS);

JUST_API void frameBufferTriangle(FrameBuffer* FRAME, TerminalPos POINT_A, TerminalPos POINT_B, TerminalPos POINT_C);

JUST_API void frameBufferTriangleFilled(FrameBuffer* FRAME, TerminalPos POINT_A, TerminalPos POINT_B, TerminalPos POINT_C);
