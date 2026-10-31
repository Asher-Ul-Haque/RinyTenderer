/**
 * @file : Camera.h
 * @brief : Camera API
 */

#pragma once
#include <cglm/cam.h>
#include <cglm/struct/cam.h>
#include <rinyTenderer/_structs.h>


/**
 * @brief : Initializes a camera
 * @param CAMERA : The camera to initialize
 * @param POSITION : Where is the camera
 */
JUST_API void cameraInit(TerminalCamera* CAMERA);

/**
 * @brief : Computes view matrix
 * @param CAMERA : The camera for which the matrix is to be calculated
 * @return : a mt4 as view matrix
 */
JUST_LOCAL static inline mat4s camera3DGetViewMatrix(TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  return glms_lookat(CAMERA->position, CAMERA->target, CAMERA->up);
}

/**
 * @brief : Computes projection matrix
 * @param CAMERA : The camera for which the matrix is to be calculated
 * reurn : a mat4 as projection matrix
 */
JUST_LOCAL static inline mat4s camera3DGetProjectionMatrix(TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  return glms_perspective(CAMERA->fov, CAMERA->aspectRatio, CAMERA->nearPlane, CAMERA->farPlane);
}

/**
 * @brief : Sets camera position
 * @param CAMERA : The camera to move
 * @param POSITION : Where to move it to
 */
JUST_API static inline void cameraSetPosition(TerminalCamera* CAMERA, FramePos POSITION)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->position = POSITION;
}

/**
 * @brief : Sets camera target
 * @param CAMERA : The camera to point
 * @param TARGET : Where to aim
 */
JUST_API static inline void cameraSetTarget(TerminalCamera* CAMERA, FramePos TARGET)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->target = TARGET; 
}

/**
 * @brief : Tells where up is
 * @param CAMERA : The camera to be informed
 * @param UP : Where is up
 */
JUST_API static inline void cameraSetUp(TerminalCamera* CAMERA, FramePos UP)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->up = UP;
}

/**
 * @brief : Set aspect ratio of camera
 * @param CAMERA : Which camera to set ratio on
 * @parma ASPECT_RATIO : What is the ratio
 */
JUST_API static inline void cameraSetAspectRatio(TerminalCamera* CAMERA, float ASPECT_RATIO)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->aspectRatio = ASPECT_RATIO;
}

/**
 * @brief : Sets camera perspective
 * @parma CAMERA : THe camera to set perspective on
 * @param FOV_DEGREES : The fov in degrees
 * @param NEAR_PLANE : near plane for frustum culling
 * @param FAR_PLANE : far plane for frustum culling
 */
JUST_API static inline void cameraSetPerspective(TerminalCamera* CAMERA, float FOV_DEGREES, float NEAR_PLANE, float FAR_PLANE)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);
  CAMERA->fov       = glm_rad(FOV_DEGREES);
  CAMERA->nearPlane = NEAR_PLANE;
  CAMERA->farPlane  = FAR_PLANE;
}
