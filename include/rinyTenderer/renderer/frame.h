/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies
 */

#pragma once
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/camera.h>
#include <rinyTenderer/renderer/light.h>
#include <rinyTenderer/_structs.h>
#include <stdint.h>




/// @brief : Pass this for the frame to occupy all the window
#define FULLSCREEN_FRAME_SIZE (TerminalPos) { .x = 0, .y = 0 }

/// @brief : Pass this for the frame to be at the top left of the window 
#define TOP_LEFT_FRAME_POS (TerminalPos) { .x = 0, .y = 0 } 

// - - - | API | - - -


// - - - Life cycle
/**
 * @brief : Initializes the frame buffer and allocates memory
 * @param FRAME : Which frame to initialize
 * @param POSITION : What is the top left of the frame
 * @param SIZE : What is the size of the frame
 * @warning : CRASHES on double init or out of bounds init
 */
JUST_API void frameCreateAbsolute(TerminalFrame* FRAME, TerminalPos POSITION, TerminalSize SIZE);

/**
 * @brief : Initializes the frame buffer and allocates memory
 * @param FRAME : Which frame to initialize
 * @param REL_POSITION : What is the top left of the frame (0.0f -> 1.0f);
 * @param REL_SIZE : What is the size of the frame relatively (0.0f -> 1.0f)
 * @warning : CRASHES on double init or out of bounds init
 */
JUST_API void frameCreateRelative(TerminalFrame* FRAME, TerminalPosRel REL_POS, TerminalSizeRel REL_SIZE);

/**
 * @brief : Destroys a frame and frees memory
 * @param FRAME : Which frame to destroy 
 * @warning : CRASHES On bad frame or double destroy
 */
JUST_API void frameDestroy(TerminalFrame* FRAME);

/**
  @brief : Checks the terminal window has resized and if so resizes the frame
  @warning : Removes all data from the frame, if running at 30 fps, one of those will be empty
*/
JUST_API void frameResize(TerminalFrame* FRAME);


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
    TerminalFrame*    FRAME, 
    FramePos          START_POS, 
    FramePos          END_POS, 
    RGBcolor          START_COLOR,
    RGBcolor          END_COLOR);

/**
 * @brief : Draws a triangle in the buffer
 * @param FRAME : Which frame to draw in 
 * @param POINT_A : Vertex A of triangle
 * @param POINT_B : Vertex B of triangle
 * @param POINT_C : Vertex C of triangle
 * @parma PIXEL_TEMPLATE : Pixels this triangle would be made of
 */
JUST_API void frameTriangle(
    TerminalFrame*    FRAME, 
    FramePos          POINT_A, 
    FramePos          POINT_B, 
    FramePos          POINT_C, 
    RGBcolor          COLORS[3]);

// - - - Color and light
/**
 * @brief : Converts a char and rgb color to terminal color space 
 * @param R : Red
 * @param G : Green
 * @param B : Blue
 */
JUST_API TerminalColor terminalColorFromRGB(uint8_t R, uint8_t G, uint8_t B);

