#pragma once

/**
 * @file : structs.h
 * @brief : Stores common structs
*/

#include <cglm/types-struct.h>
#include <justLibrary.h>
#include <rinyTenderer/window.h>

/// @brief : A frame position is just 3 float vector
typedef vec3s     FramePos;

/// @brief : A direction is just 3 float vector
typedef vec3s     FrameDir;

/// @brief : A rotation is just 3 float vector
typedef vec3s     FrameRotation;

/// @brief : Just an index
typedef uint8_t TerminalLightIndex;

/// @brief : 8 bit color
typedef uint8_t TerminalColor;

/// @brief : Standard RGB 3 byte color
typedef struct rgbColor
{
  uint8_t r;  ///< Red
  uint8_t g;  ///< Green
  uint8_t b;  ///< Blue
} RGBcolor;

/// @brief : Directional and Point lights 
typedef struct terminalLight3D
{
  RGBcolor color;     ///< The color the light emits
  uint16_t intensity; ///< The intensity of this light

  union
  {
    FrameDir direction;  ///< Used if directional light
    FramePos position;   ///< Used if point light
  };
} TerminalLight;

/// @brief : The vertex of a triangle in a mesh
typedef struct terminalVertex3D
{
  FramePos position; ///< 3D coordinates in model space (x, y, z)
  RGBcolor color;    ///< Base color or vertex attribute
} TerminalVertex;

/// @brief : A triangle is 3 indices to vertices
typedef struct terminalTriangle3D
{
  uint16_t indices[3];  ///< Indices to the terminal vertex
} TerminalTriangle;

/// @brief : A transformation 
typedef struct transform3D
{
  FramePos      position; ///< World positions (x, y, z)
  FrameRotation rotation; ///< Euler angles (pitch, yaw, roll in radians)
  FrameDir      scale;    ///< Scaling factors (x, y, x), default to (1, 1, 1)
} Transform3D;

/// @brief : A mesh is just vertices, colors and triangles and a singular transform
typedef struct terminalMesh3D
{
  TerminalVertex*   vertices;       ///< All the vertices
  size_t            vertexCount;    ///< How many vertices

  TerminalTriangle* triangles;      ///< All the triangles
  size_t            triangleCount;  ///< How many triangles

  Transform3D       transform;      ///< How to transform this mesh
} TerminalMesh;

/// @brief : A perspective camera
typedef struct terminalCamera3D
{
  FramePos position;     ///< Where the camera is
  FramePos target;       ///< Which way is it looking
  FrameDir up;           ///< Which way is up

  float fov;          ///< Field of view in degrees
  float nearPlane;    ///< Near plane for frustum culling
  float farPlane;     ///< Far plane for frustum culling

  float aspectRatio;  ///< Aspect ratio
} TerminalCamera;

#define MAX_FRAME_LIGHTS 5

/// @brief : Say a frame in a window, a window can have multiple frames
typedef struct terminalFrame
{
  TerminalSize    position;       ///< Where in the terminal
  TerminalSize    size;           ///< How big is the terminal, use this for operations

  TerminalColor*  backBuffer;     ///< Where to draw 
  TerminalColor*  frontBuffer;    ///< What to show
  uint16_t*       depthBuffer;    ///< How deep

  struct timespec lastFrameTime;  ///< Last frame stat
  uint8_t         targetFPS;      ///< duh
  uint8_t         actualFPS;      ///< duh

  TerminalCamera  camera;         ///< The camera                                  

  RGBcolor        ambientColor;                           ///< Global ambient color
  float           ambientIntensity;                       ///< Global ambient intensity multiplier

  TerminalLight   directionalLights[MAX_FRAME_LIGHTS];    ///< Fixed array of light sources
  uint8_t         directionalLightCount;                  ///< How many lights

  TerminalLight   pointLights[MAX_FRAME_LIGHTS];          ///< Fixed array of light sources                                                       
  uint8_t         pointLightCount;                        ///< How many point lights                                                       
} TerminalFrame;
