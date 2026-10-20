#pragma once

#include <cglm/struct/vec3.h>
#include <cglm/types-struct.h>
#include <justLibrary.h>
#include <stdint.h>
#include <rinyTenderer/renderer/_structs.h>

static inline TerminalLight lightCreateDirectional(FramePos DIRECTION, RGBcolor COLOR, float INTENSITY)
{
  return (TerminalLight)
    {
      .color     = COLOR,
      .intensity = INTENSITY,
      .direction = glms_vec3_normalize(DIRECTION)
    };
}

static inline TerminalLight lightCreatePoint(FramePos POSITION, RGBcolor COLOR, float INTENSITY)
{
  return (TerminalLight)
    {
      .color     = COLOR,
      .intensity = INTENSITY,
      .position  = POSITION
    };
}

/**
 * @brief : Adds a directional light
 * @param FRAME : The frame to add light to
 * @param LIGHT : The light to be added
 * @return : an index to the light added if successful, 0xFF if failed
 */
JUST_API TerminalLightIndex frameAddDirectionalLight(TerminalFrame* FRAME, TerminalLight LIGHT);

/**
 * @brief : Adds a point light
 * @param FRAME : The frame to add light to
 * @param LIGHT : The light to be added
 * @return : an index to the light added if successful, 0xFF if failed
 */
JUST_API TerminalLightIndex frameAddPointLight(TerminalFrame* FRAME, TerminalLight LIGHT);

/**
 * @brief : Removes a directional light
 * @param FRAME : The frame to remove light from
 * @param INDEX : The light to be removed
 */
JUST_API void frameRemoveDirectionalLight(TerminalFrame* FRAME, TerminalLightIndex INDEX);

/**
 * @brief : Removes a point light
 * @param FRAME : The frame to remove light from
 * @param INDEX : The light to be removed
 */
JUST_API void frameRemovePointLight(TerminalFrame* FRAME, TerminalLightIndex INDEX);
