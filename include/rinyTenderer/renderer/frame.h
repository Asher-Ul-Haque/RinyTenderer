/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies
 */

#pragma once
#include "justLibrary.h"
#include <rinyTenderer/window.h>

/// @brief : Say a frame in a window, a window can have multiple frames
typedef struct framebuffer
{
  TerminalSize  position; ///< Where in the terminal
  TerminalSize  size;     ///< How big is the terminal
  char*         buffer;   ///< Memory
} FrameBuffer;

/// @brief : Pass this for the frame to occupy all the window
#define BIGGEST_FRAME_SIZE (TerminalSize) { .rows = 0, .cols = 0 }

/// @brief : Pass this for the frame to be at the top left of the window 
#define TOP_LEFT_FRAME_POS (TerminalSize) { .rows = 1, .cols = 1 } 

JUST_API void frameBufferInit(FrameBuffer* FRAME, TerminalSize POSITION, TerminalSize SIZE);

JUST_API void frameBufferDestroy(FrameBuffer* FRAME);

JUST_API void frameBufferRender(const FrameBuffer* FRAME);

JUST_API void frameBufferSetChar(FrameBuffer* FRAME, TerminalSize POSITION);
