/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies
 */

#pragma once
#include "justLibrary.h"
#include <rinyTenderer/window.h>
#include <stdint.h>

/// @brief : A terminalPixel
typedef struct terminalPixel
{
  char    ch;
  uint8_t color;
} TerminalPixel;

/// @brief : Say a frame in a window, a window can have multiple frames
typedef struct framebuffer
{
  TerminalSize    position;       ///< Where in the terminal
  TerminalSize    size;           ///< How big is the terminal
  TerminalPixel*  backBuffer;     ///< Where to draw 
  TerminalPixel*  frontBuffer;    ///< What to show
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
#define DEFAULT_TARGET_FPS 30

// - - - | API | - - -


/**
 * @brief : Initializes the frame buffer and allocates memory
 * @param FRAME : Which frame to initialize
 * @param POSITION : What is the top left of the frame
 * @param SIZE : What is the size of the frame
 * @warning : CRASHES on double init or out of bounds init
 */
JUST_API void frameBufferInit(FrameBuffer* FRAME, TerminalPos POSITION, TerminalSize SIZE);

/**
 * @brief : Destroys a frame and frees memory
 * @param FRAME : Which frame to destroy 
 * @warning : CRASHES On bad frame or double destroy
 */
JUST_API void frameBufferDestroy(FrameBuffer* FRAME);

/**
 * @brief : Sets the frame target fps
 * @param FRAME : Which frame
 * @param TARGET_FPS : The actual target
 */
JUST_API static inline void frameBufferSetFPS(FrameBuffer* FRAME, uint8_t TARGET_FPS)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  FRAME->targetFPS = TARGET_FPS;
}

/**
 * @brief : Returns the frame actual fps
 * @param FRAME : Which frames fps to get
 * @return : the actual fps 
 */
JUST_API static inline uint8_t frameBufferGetFPS(const FrameBuffer* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  return FRAME->actualFPS;
}

/**
 * @brief : Renders a frame
 * @param FRAME : Which frame to render
 */
JUST_API void frameBufferRender(FrameBuffer* FRAME);

/**
 * @brief : Clears a frame buffer
 * @param FRAME : Which frame to clear
 * @param PIXEL_TEMPLATE : What to clear with 
 */
JUST_API void frameBufferClear(FrameBuffer* FRAME, TerminalPixel PIXEL_TEMPLATE);

/**
 * @brief : Sets a specific pixel in the frame
 * @param FRAME : Which frame
 * @param POSITION : Where
 * @param PIXEL : What
 */
JUST_API void frameBufferSetPixel(FrameBuffer* FRAME, TerminalPos POSITION, TerminalPixel PIXEL);

/**
 * @brief : Draws a line in the buffer
 * @param FRAME : Which frame to draw in
 * @param START_POS : The line start pos
 * @param END_POS : The line end pos
 * @param PIXEL_TEMPLATE : Pixels this line would be made of
 */
JUST_API void frameBufferLine(
    FrameBuffer*  FRAME, 
    TerminalPos   START_POS, 
    TerminalPos   END_POS, 
    TerminalPixel PIXEL_TEMPLATE);

/**
 * @brief : Draws a triangle in the buffer
 * @param FRAME : Which frame to draw in 
 * @param POINT_A : Vertex A of triangle
 * @param POINT_B : Vertex B of triangle
 * @param POINT_C : Vertex C of triangle
 * @parma PIXEL_TEMPLATE : Pixels this triangle would be made of
 */
JUST_API void frameBufferTriangle(
    FrameBuffer*  FRAME, 
    TerminalPos   POINT_A, 
    TerminalPos   POINT_B, 
    TerminalPos   POINT_C, 
    TerminalPixel PIXEL_TEMPLATE);

/**
 * @brief : Converts a char and rgb color to terminal color space 
 * @param CHAR : The char
 * @param R : Red
 * @param G : Green
 * @param B : Blue
 */
JUST_API TerminalPixel terminalPixelFromColor(char CHAR, uint8_t R, uint8_t G, uint8_t B);
