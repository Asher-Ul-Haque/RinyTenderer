#pragma once
#include "justLibrary.h"
#include <cglm/struct.h>
#include <rinyTenderer/renderer/frame.h>

typedef struct terminalVertex3D
{
  vec3s         position; ///< 3D coordinates in model space (x, y, z)
  TerminalColor color;    ///< Base color or vertex attribute
} TerminalVertex;

typedef struct terminalTriangle3D
{
  uint16_t indices[3];  ///< Indices to the terminal 
} TerminalTriangle;

typedef struct transform3D
{
  vec3s position; ///< World positions (x, y, z)
  vec3s rotation; ///< Euler angles (pitch, yaw, roll in radians)
  vec3s scale;    ///< Scaling factors (x, y, x), default to (1, 1, 1)
} Transform3D;

typedef struct terminalMesh3D
{
  TerminalVertex* vertices;
  size_t          vertexCount;

  TerminalTriangle* triangles;
  size_t            triangleCount;

  Transform3D       transform;
} TerminalMesh;


JUST_API void mesh3DCreate(TerminalMesh* MESH, size_t VERTEX_COUNT, size_t TRIANGLE_COUNT);

JUST_API void mesh3DDestroy(TerminalMesh* MESH);

JUST_API void frameBufferDrawMesh(FrameBuffer* FRAME, const TerminalMesh* MESH);
