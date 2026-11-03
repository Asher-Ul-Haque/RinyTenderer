#pragma once
#include <rinyTenderer/renderer/frame.h>

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

