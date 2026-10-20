/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies
 */

#pragma once
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/camera.h>
#include <rinyTenderer/renderer/light.h>
#include <rinyTenderer/renderer/_structs.h>
#include <stdint.h>




/// @brief : Pass this for the frame to occupy all the window
#define FULLSCREEN_FRAME_SIZE (TerminalPos) { .cols = 0, .rows = 0 }

/// @brief : Pass this for the frame to be at the top left of the window 
#define TOP_LEFT_FRAME_POS (TerminalPos) { .x = 1, .y = 1 } 

/// @brief : No limits on fps, I paid for the entire CPU
#define UNCAPPED_FRAME_FPS 0

/// @brief : default target fps 
#define DEFAULT_TARGET_FPS 30


// - - - | API | - - -


// - - - Life cycle
/**
 * @brief : Initializes the frame buffer and allocates memory
 * @param FRAME : Which frame to initialize
 * @param POSITION : What is the top left of the frame
 * @param SIZE : What is the size of the frame
 * @warning : CRASHES on double init or out of bounds init
 */
JUST_API void frameCreate(TerminalFrame* FRAME, TerminalPos POSITION, TerminalSize SIZE);

/**
 * @brief : Destroys a frame and frees memory
 * @param FRAME : Which frame to destroy 
 * @warning : CRASHES On bad frame or double destroy
 */
JUST_API void frameDestroy(TerminalFrame* FRAME);


// - - - FPS
/**
 * @brief : Sets the frame target fps
 * @param FRAME : Which frame
 * @param TARGET_FPS : The actual target
 */
JUST_API static inline void frameSetFPS(TerminalFrame* FRAME, uint8_t TARGET_FPS)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  FRAME->targetFPS = TARGET_FPS;
}

/**
 * @brief : Returns the frame actual fps
 * @param FRAME : Which frames fps to get
 * @return : the actual fps 
 */
JUST_API static inline uint8_t frameGetFPS(const TerminalFrame* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  return FRAME->actualFPS;
}


// - - - Draw and clear
/**
 * @brief : Renders a frame
 * @param FRAME : Which frame to render
 */
JUST_API void frameRender(TerminalFrame* FRAME);

/**
 * @brief : Clears a frame buffer
 * @param FRAME : Which frame to clear
 * @param PIXEL_TEMPLATE : What to clear with 
 */
JUST_API void frameClear(TerminalFrame* FRAME, TerminalColor PIXEL_TEMPLATE);

// - - - Basic geometry
/**
 * @brief : Sets a specific pixel in the frame
 * @param FRAME : Which frame
 * @param POSITION : Where
 * @param PIXEL : What
 */
JUST_API void frameSetPixel(TerminalFrame* FRAME, FramePos POSITION, TerminalColor PIXEL);

/**
 * @brief : Draws a line in the buffer
 * @param FRAME : Which frame to draw in
 * @param START_POS : The line start pos
 * @param END_POS : The line end pos
 * @param PIXEL_TEMPLATE : Pixels this line would be made of
 */
JUST_API void frameLine(
    TerminalFrame*  FRAME, 
    FramePos        START_POS, 
    FramePos        END_POS, 
    TerminalColor   PIXEL_TEMPLATE);

/**
 * @brief : Draws a triangle in the buffer
 * @param FRAME : Which frame to draw in 
 * @param POINT_A : Vertex A of triangle
 * @param POINT_B : Vertex B of triangle
 * @param POINT_C : Vertex C of triangle
 * @parma PIXEL_TEMPLATE : Pixels this triangle would be made of
 */
JUST_API void frameTriangle(
    TerminalFrame*  FRAME, 
    FramePos        POINT_A, 
    FramePos        POINT_B, 
    FramePos        POINT_C, 
    TerminalColor   PIXEL_TEMPLATE);

// - - - Color and light
/**
 * @brief : Converts a char and rgb color to terminal color space 
 * @param R : Red
 * @param G : Green
 * @param B : Blue
 */
JUST_API TerminalColor terminalColorFromRGB(uint8_t R, uint8_t G, uint8_t B);

