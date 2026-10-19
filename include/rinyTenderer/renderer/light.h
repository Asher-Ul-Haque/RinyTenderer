#pragma once

#include "cglm/struct/vec3.h"
#include <cglm/types-struct.h>
#include <justLibrary.h>
#include <stdint.h>

/// @brief : A terminalPixel
typedef uint8_t terminalColor;
typedef uint8_t TerminalColor;

typedef struct rgbColor
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RGBcolor;

typedef struct terminalLight
{
  RGBcolor          color;
  uint16_t          intensity;

  union
  {
    vec3s direction;  ///< Used if directional light
    vec3s position;   ///< Used if point light
  };
} TerminalLight;

static inline TerminalLight lightCreateDirectional(vec3s DIRECTION, RGBcolor COLOR, float INTENSITY)
{
  return (TerminalLight)
  {
    .color     = COLOR,
    .intensity = INTENSITY,
    .direction = glms_vec3_normalize(DIRECTION)
  };
}

static inline TerminalLight lightCreatePoint(vec3s POSITION, RGBcolor COLOR, float INTENSITY)
{
  return (TerminalLight)
  {
    .color     = COLOR,
    .intensity = INTENSITY,
    .position  = POSITION
  };
}
