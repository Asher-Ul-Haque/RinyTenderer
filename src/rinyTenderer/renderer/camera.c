#include "justLibrary.h"
#include <rinyTenderer/renderer/camera.h>

JUST_API void cameraInit(TerminalCamera* CAMERA)
{
  JUST_ASSERT_DEBUG(CAMERA != NULL);

  // - - - Be at 0, 0, 5
  FramePos cameraPos = { { 0, 0, 5 } };
  cameraSetPosition(CAMERA, cameraPos);

  // - - - Look at the world origin
  FramePos origin = { { 0.0f, 0.0f, 0.0f } };
  cameraSetTarget(CAMERA, origin);

  // - - - Up is the y axis
  FramePos yAxis = { { 0.0f, 1.0f, 0.0f } };
  cameraSetUp(CAMERA, yAxis);

  // - - - Reasonable defaults 
  float defaultFOV          = 60;
  float defaultNearPlane    = 0.1f;
  float defaultFarPlane     = 100.0f;
  float defaultAspectRatio  = 1.0f;
  cameraSetPerspective(CAMERA, defaultFOV, defaultNearPlane, defaultFarPlane);
  cameraSetAspectRatio(CAMERA, defaultAspectRatio);
}
