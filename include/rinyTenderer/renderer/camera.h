#pragma once
#include <cglm/struct.h>
#include <justLibrary.h>

typedef struct terminalCamera3D
{
  vec3s position;
  vec3s target;
  vec3s up;

  float fov;
  float nearPlane;
  float farPlane;

  float aspectRatio;
} TerminalCamera;

JUST_API void cameraInit(TerminalCamera* CAMERA);

JUST_LOCAL static inline mat4s camera3DGetViewMatrix(const TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  return glms_lookat(CAMERA->position, CAMERA->target, CAMERA->up);
}

JUST_LOCAL static inline mat4s camera3DGetProjectionMatrix(const TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  return glms_perspective(CAMERA->fov, CAMERA->aspectRatio, CAMERA->nearPlane, CAMERA->farPlane);
}

JUST_API static inline void cameraSetPosition(TerminalCamera* CAMERA, vec3s POSITION)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->position = POSITION;
}

JUST_API static inline void cameraSetTarget(TerminalCamera* CAMERA, vec3s TARGET)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->target = TARGET;
}

JUST_API static inline void cameraSetUp(TerminalCamera* CAMERA, vec3s UP)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->up = UP;
}

JUST_API static inline void cameraSetAspectRatio(TerminalCamera* CAMERA, float ASPECT_RATIO)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->aspectRatio = ASPECT_RATIO;
}

JUST_API static inline void cameraSetPerspective(TerminalCamera* CAMERA, float FOV_DEGREES, float NEAR_PLANE, float FAR_PLANE)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->fov       = glm_rad(FOV_DEGREES);
  CAMERA->nearPlane = NEAR_PLANE;
  CAMERA->farPlane  = FAR_PLANE;
}
