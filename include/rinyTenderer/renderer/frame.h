/**
 * @file : frame.h
 * @brief : Handles framebuffer and viewport thingies with unified depth-tested cells
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

#define CELL_SIZE_CHARS 4

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
 * @brief : Checks the terminal window has resized and if so resizes the frame
 * @warning : Removes all data from the frame, if running at 30 fps, one of those will be empty
 */
JUST_API void frameResize(TerminalFrame* FRAME);


// - - - Draw and clear
/**
 * @brief : Renders a frame
 * @param FRAME : Which frame to render
 */
JUST_API void frameRender(TerminalFrame* FRAME);

/**
 * @brief : Clears a frame buffer and depth buffer
 * @param FRAME : Which frame to clear
 * @param PIXEL_TEMPLATE : What color/template to clear background with 
 */
JUST_API void frameClear(TerminalFrame* FRAME, TerminalColor PIXEL_TEMPLATE);


// - - - Basic geometry and text
/**
 * @brief : Sets a specific pixel/mesh fragment in the frame with depth testing
 * @param FRAME : Which frame
 * @param POSITION : Where (FramePos / vec3s)
 * @param COLOR : Pixel color
 * @param DEPTH : Depth value for depth testing
 */
JUST_API void frameSetPixel(
    TerminalFrame*  FRAME, 
    FramePos        POSITION, 
    TerminalColor   COLOR);

/**
 * @brief : Sets a specific character/text cell in the frame with foreground, background, and depth testing
 * @param FRAME : Which frame
 * @param POSITION : Where (FramePos / vec3s; y-coordinate snaps to even rows for text alignment)
 * @param CHAR : Raw UTF-8 character string (up to 4 bytes)
 * @param FG_COLOR : Foreground text color
 * @param BG_COLOR : Background cell color
 */
JUST_API void frameSetChar(
    TerminalFrame*  FRAME, 
    FramePos        POSITION, 
    const char*     CHAR, 
    TerminalColor   FG_COLOR, 
    TerminalColor   BG_COLOR);
