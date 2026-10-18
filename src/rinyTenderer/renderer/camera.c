#include <rinyTenderer/renderer/camera.h>

JUST_API void cameraInit(TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);

  CAMERA->position  = (vec3s) {{ 0.0f, 0.0f, 5.0f }}; ///< Default position back from origin
  CAMERA->target    = (vec3s) {{ 0.0f, 0.0f, 0.0f }}; ///< Looking at the world origin
  CAMERA->up        = (vec3s) {{ 0.0f, 1.0f, 0.0f }}; ///< Up in y axis                                                   

  CAMERA->fov         = glm_rad(60.0f);
  CAMERA->nearPlane   = 0.1f;
  CAMERA->farPlane    = 100.0f;
  CAMERA->aspectRatio = 1.0f;
}
