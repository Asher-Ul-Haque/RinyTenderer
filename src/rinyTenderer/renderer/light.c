#include <rinyTenderer/renderer/frame.h>

JUST_API TerminalLightIndex frameAddDirectionalLight(TerminalFrame* FRAME, TerminalLight LIGHT)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (FRAME->directionalLightCount >= MAX_FRAME_LIGHTS)
  {
    JUST_LOG_ERROR("[FRAME] : Cannot add more lights, maximum capacity %d reached", MAX_FRAME_LIGHTS);
    return 0xFF;
  }

  FRAME->directionalLights[FRAME->directionalLightCount++] = LIGHT;
  return FRAME->directionalLightCount - 1;
}

JUST_API TerminalLightIndex frameAddPointLight(TerminalFrame* FRAME, TerminalLight LIGHT)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (FRAME->pointLightCount >= MAX_FRAME_LIGHTS)
  {
    JUST_LOG_ERROR("[FRAME] : Cannot add more lights, maximum capacity %d reached", MAX_FRAME_LIGHTS);
    return 0xFF;
  }

  FRAME->pointLights[FRAME->pointLightCount++] = LIGHT;
  return FRAME->pointLightCount - 1;
}

JUST_API void frameRemoveDirectionalLight(TerminalFrame* FRAME, TerminalLightIndex INDEX)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (INDEX >= FRAME->directionalLightCount)
  {
    JUST_LOG_ERROR("[FRAME] : Cannot remove light %d , out of bounds %d", INDEX, FRAME->directionalLightCount);
    return;
  }

  FRAME->directionalLights[INDEX] = FRAME->directionalLights[FRAME->directionalLightCount - 1];
  FRAME->directionalLightCount--;
  return;
}

JUST_API void frameRemovePointLight(TerminalFrame* FRAME, TerminalLightIndex INDEX)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);

  if (INDEX >= FRAME->pointLightCount)
  {
    JUST_LOG_ERROR("[FRAME] : Cannot remove light %d , out of bounds %d", INDEX, FRAME->pointLightCount);
    return;
  }

  FRAME->pointLights[INDEX] = FRAME->pointLights[FRAME->pointLightCount - 1];
  FRAME->pointLightCount--;
  return;
}
