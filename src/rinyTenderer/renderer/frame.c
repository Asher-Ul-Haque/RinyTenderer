#include "rinyTenderer/_structs.h"
#include <bits/time.h>
#include <justLibrary.h>
#include <rinyTenderer/window.h>
#include <rinyTenderer/renderer/frame.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


// - - - Color - - -

JUST_API TerminalColor terminalColorFromRGB(uint8_t R, uint8_t G, uint8_t B)
{
  // - - - Allow a small channel tolerance (+-3)
  if (abs((int)R - (int)G) <= 3 && abs((int)G - (int)B) <= 3)
  {
    if (R < 4)   return 16;  // Black
    if (R > 243) return 231; // Pure White
    
    // - - - Map R in [8, 238] to 24 steps (code 232 to 255)
    int grayIdx = (R - 3) / 10;
    if (grayIdx < 0)  grayIdx = 0;
    if (grayIdx > 23) grayIdx = 23;
    return 232 + grayIdx;
  }

  // - - - Rounding for 6x6x6 color cube levels [0..5]
  int ir = (R * 5 + 127) / 255;
  int ig = (G * 5 + 127) / 255;
  int ib = (B * 5 + 127) / 255;

  return (uint8_t)(16 + (36 * ir) + (6 * ig) + ib);
}

static inline TerminalSize resolveActualSize(TerminalSizeRel REL_SIZE)
{
  vec2s windowAbsSize = terminalGetContext()->size;
  float termCols      = windowAbsSize.x;
  float termRows      = windowAbsSize.y;
  
  uint16_t width  = (REL_SIZE.x <= 0.0f) ? (uint16_t)termCols : (uint16_t)(termCols * REL_SIZE.x);
  uint16_t height = (REL_SIZE.y <= 0.0f) ? (uint16_t)termRows : (uint16_t)(termRows * REL_SIZE.y);
  
  return (TerminalSize)
    {
      .x = (float)width, 
      .y = (float)height 
    };
}

static inline TerminalPos resolveActualPos(TerminalPosRel REL_POS)
{
  vec2s windowAbsSize = terminalGetContext()->size;
  return (TerminalPos)
    {
      .x = (float)(uint16_t)(windowAbsSize.x * REL_POS.x), 
      .y = (float)(uint16_t)(windowAbsSize.y * REL_POS.y) 
    };
}

// - - - Create and Destroy - - - 

JUST_API void frameCreateAbsolute(TerminalFrame* FRAME, TerminalPos POSITION, TerminalSize SIZE)
{
  JUST_LOG_DEBUG("[FRAME] : Creating a new frame");

  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot init a NULL FRAME");

  const TerminalCTX* ctx = terminalGetContext();

  if (SIZE.x == 0 || SIZE.y == 0)
  {
    JUST_LOG_WARNING("[FRAME] : Going for maximimum size");
    FRAME->size = ctx->size;
  }
  else
  {
    FRAME->size = SIZE;
  }
  FRAME->position = POSITION;

  // - - - allocate 1D buffer : ((width + 1 for \n) * height) + 1 for \0
  size_t width      = FRAME->size.x;
  size_t height     = FRAME->size.y;
  size_t totalCells = width * height;
  
  JUST_LOG_DEBUG(
    "[FRAME] : Size = %u cols x %u rows",
    (unsigned)FRAME->size.y,
    (unsigned)FRAME->size.x);

  FRAME->backBuffer = (TerminalColor*) JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor), RINY_TENDERER_TAG);
  JUST_ASSERT_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  FRAME->frontBuffer = (TerminalColor*) JUST_MALLOC_TAGGED(totalCells * sizeof(TerminalColor), RINY_TENDERER_TAG);
  JUST_ASSERT_MESSAGE(FRAME->frontBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  FRAME->depthBuffer = (uint16_t*) JUST_MALLOC_TAGGED(totalCells * sizeof(uint16_t), RINY_TENDERER_TAG);
  JUST_ASSERT_MESSAGE(FRAME->depthBuffer != NULL, "[FRAME] : Allocation for this frame failed");

  // - - - refill
  memset(FRAME->backBuffer,  0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->frontBuffer, 0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->depthBuffer, 0xFF, totalCells * sizeof(uint16_t));

  // - - - setup camera
  cameraInit(&FRAME->camera);
  float aspectRatio = (float) FRAME->size.x / (float) FRAME->size.y;
  cameraSetAspectRatio(&FRAME->camera, aspectRatio);

  // - - - initial ambience light
  FRAME->ambientColor     = (RGBcolor) { 155.0f, 155.0f, 155.0f };
  FRAME->ambientIntensity = 1.0f;

  // - - - initial clear
  frameClear(FRAME, 0);

  JUST_LOG_INFO("[FRAME] : Frame ready");
}

JUST_API void frameCreateRelative(TerminalFrame* FRAME, TerminalPosRel REL_POS, TerminalSizeRel REL_SIZE)
{
  JUST_LOG_DEBUG("[FRAME] : Creating relative frame");
  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot init a NULL FRAME");

  const TerminalCTX* ctx = terminalGetContext();

  vec2s actualSize = resolveActualSize(REL_SIZE);
  vec2s actualPos  = resolveActualPos(REL_POS); 

  frameCreateAbsolute(FRAME, actualPos, actualSize);

  FRAME->widthRatio = (ctx->size.x > 0) ? (float)actualSize.x / (float)ctx->size.x : 1.0f;
  FRAME->relPos     = REL_POS;

  JUST_LOG_INFO("[FRAME] : Relative frame ready (%u x %u)", (unsigned)actualSize.x, (unsigned)actualSize.y);
}

JUST_API void frameDestroy(TerminalFrame* FRAME)
{
  JUST_LOG_DEBUG("[FRAME] : Destroying a FRAME");
  JUST_ASSERT_DEBUG_MESSAGE(FRAME != NULL, "[FRAME] : Cannot destroy a NULL FRAME");

  JUST_FREE(FRAME->backBuffer);
  JUST_FREE(FRAME->frontBuffer);
  JUST_FREE(FRAME->depthBuffer);

  FRAME->backBuffer  = NULL;
  FRAME->frontBuffer = NULL;
  FRAME->depthBuffer = NULL;

  JUST_LOG_INFO("[FRAME] : Destroyed");
}

JUST_API void frameResize(TerminalFrame* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot resize, unitiialized FRAME");

  const TerminalCTX* ctx = terminalGetContext();

  // - - - calculate new sizes 
  uint16_t  newWidth  = (uint16_t) (ctx->size.x * FRAME->widthRatio);
  float     aspect    = (float) FRAME->size.x / (float) FRAME->size.y;
  uint16_t  newHeight = (uint16_t) ((float) newWidth / aspect);

  // - - - maintain a non-zero size
  if (newWidth < 1)  newWidth  = 1;
  if (newHeight < 1) newHeight = 1;

  // - - - calculate new pos
  vec2s newPos = 
  {
    .x = (uint16_t) (ctx->size.x * FRAME->relPos.x),
    .y = (uint16_t) (ctx->size.y * FRAME->relPos.y)
  };

  if (newWidth  == FRAME->size.x      &&
      newHeight == FRAME->size.y      &&
      newPos.x  == FRAME->position.x  &&
      newPos.y  == FRAME->position.y)
  { return; }

  // - - - assign new size and pos
  FRAME->size     = (TerminalSize)
    {
      .x = (float)newWidth, 
      .y = (float)newHeight 
    };
  FRAME->position = newPos;

  // - - - realloc
  size_t totalCells = (size_t) newWidth * (size_t) newHeight;

  FRAME->backBuffer   = JUST_REALLOC(FRAME->backBuffer,  totalCells * sizeof(TerminalColor));
  FRAME->frontBuffer  = JUST_REALLOC(FRAME->frontBuffer, totalCells * sizeof(TerminalColor));
  FRAME->depthBuffer  = JUST_REALLOC(FRAME->depthBuffer, totalCells * sizeof(uint16_t));

  JUST_ASSERT_DEBUG(FRAME->backBuffer  != NULL);
  JUST_ASSERT_DEBUG(FRAME->frontBuffer != NULL);
  JUST_ASSERT_DEBUG(FRAME->depthBuffer != NULL);

  // - - - refill
  memset(FRAME->backBuffer,  0,    totalCells * sizeof(TerminalColor));
  memset(FRAME->frontBuffer, 0xFF, totalCells * sizeof(TerminalColor));
  memset(FRAME->depthBuffer, 0xFF, totalCells * sizeof(uint16_t));

  JUST_LOG_INFO("[FRAME] : Resized to %u x %u at (%u, %u)",
      (uint16_t)newWidth, (uint16_t)newHeight, (uint16_t)newPos.x, (uint16_t)newPos.y);
}

// - - - Render and clear - - - 

JUST_API void frameRender(TerminalFrame* FRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot render in an uninitialized buffer");

  // - - - scan through and only write cells that changed
  uint16_t width  = FRAME->size.x;
  uint16_t height = FRAME->size.y;
  uint16_t rows   = height / 2;

  for (uint16_t y = 0; y < rows; ++y)
  {
    for (uint16_t x = 0; x < width; ++x)
    {
      size_t topIndex     = (y * 2)     * width + x;
      size_t bottomIndex  = (y * 2 + 1) * width + x;

      TerminalColor topBack     = FRAME->backBuffer[topIndex];
      TerminalColor bottomBack  = FRAME->backBuffer[bottomIndex];

      TerminalColor topFront    = FRAME->frontBuffer[topIndex];
      TerminalColor bottomFront = FRAME->frontBuffer[bottomIndex];

      if (topBack != topFront || bottomBack != bottomFront)
      {
      char ch[4] = " ";
      uint8_t fg = 7;
      uint8_t bg = 0;

      bool hasTop = (topBack != 0);
      bool hasBot = (bottomBack != 0);

      if (hasTop && hasBot)
      {
        snprintf(ch, sizeof(ch), "▀");
        fg = topBack;
        bg = bottomBack;
      }
      else if (hasTop && !hasBot)
      {
        snprintf(ch, sizeof(ch), "▀");
        fg = topBack;
        bg = 0;
      }
      else if (!hasTop && hasBot)
      {
        snprintf(ch, sizeof(ch), "▄");
        fg = bottomBack;
        bg = 0;
      }
      else
      {
        snprintf(ch, sizeof(ch), " ");
        fg = 0;
        bg = 0;
      }

      printf("\033[38;5;%dm\033[48;5;%dm\033[%d;%dH%s", 
             fg, bg,
             (int) FRAME->position.y + y, 
             (int) FRAME->position.x + x, 
             ch);

        // - - - Sync front buffer
        FRAME->frontBuffer[topIndex]    = topBack;
        FRAME->frontBuffer[bottomIndex] = bottomBack;
      }
    }
  }
  fflush(stdout);
}

JUST_API void frameClear(TerminalFrame* FRAME, TerminalColor PIXEL_TEMPLATE)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot clear an uninitialized buffer");

  size_t totalCells = (size_t)FRAME->size.x * (size_t)FRAME->size.y; 

  memset(FRAME->backBuffer,  PIXEL_TEMPLATE, totalCells * sizeof(TerminalColor));
  memset(FRAME->depthBuffer, 0xFF,           totalCells * sizeof(uint16_t));
}

JUST_API void frameSetPixel(TerminalFrame* FRAME, FramePos POSITION, TerminalColor PIXEL)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(FRAME->backBuffer != NULL, "[FRAME] : Cannot draw in an uninitialized buffer");

  if (POSITION.x < 0 || POSITION.x >= FRAME->size.x) return;
  if (POSITION.y < 0 || POSITION.y >= FRAME->size.y) return;

  size_t index = POSITION.y * (FRAME->size.x) + POSITION.x;
  if (POSITION.z > FRAME->depthBuffer[index]) return;

  FRAME->backBuffer[index]  = PIXEL;
  FRAME->depthBuffer[index] = POSITION.z;
}


// - - - Geometry - - - 

JUST_API void frameLine(
    TerminalFrame*    FRAME, 
    FramePos          START_POS, 
    FramePos          END_POS, 
    RGBcolor          START_COLOR,
    RGBcolor          END_COLOR)
{
  int x0 = START_POS.x;
  int y0 = START_POS.y;
  int x1 = END_POS.x;
  int y1 = END_POS.y;
  int z0 = START_POS.z;
  int z1 = END_POS.z;

  int r0 = START_COLOR.r, g0 = START_COLOR.g, b0 = START_COLOR.b;
  int r1 = END_COLOR.r,   g1 = END_COLOR.g,   b1 = END_COLOR.b;

  // - - - Handle steep lines by swapping axes
  bool steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
  {
    int temp = x0; x0 = y0; y0 = temp;
    temp = x1;     x1 = y1; y1 = temp;
  }

  // - - - Iterate from left to right
  if (x0 > x1)
  {
    int temp = x0; x0 = x1; x1 = temp;
    temp = y0;     y0 = y1; y1 = temp;
    temp = z0;     z0 = z1; z1 = temp;

    temp = r0; r0 = r1; r1 = temp;
    temp = g0; g0 = g1; g1 = temp;
    temp = b0; b0 = b1; b1 = temp;
  }

  int dx        = x1 - x0;
  int dy        = abs(y1 - y0);
  int error     = dx / 2;
  int ystep     = (y0 < y1) ? 1 : -1;
  int y         = y0;
  int totalSteps = (dx > 0) ? dx : 1;

  for (int x = x0; x <= x1; x++)
  {
    int tNum     = x - x0;
    int currentZ = z0 + ((z1 - z0) * tNum) / totalSteps;
    int currR    = r0 + ((r1 - r0) * tNum) / totalSteps;
    int currG    = g0 + ((g1 - g0) * tNum) / totalSteps;
    int currB    = b0 + ((b1 - b0) * tNum) / totalSteps;

    FramePos position = 
    {
      .x = steep ? y : x,
      .y = steep ? x : y,
      .z = (uint16_t) currentZ
    };

    TerminalColor termColor = terminalColorFromRGB((uint8_t)currR, (uint8_t)currG, (uint8_t)currB);
    frameSetPixel(FRAME, position, termColor);

    error -= dy;
    if (error < 0)
    {
      y   += ystep;
      error += dx;
    }
  }
}

JUST_API void frameTriangle(
    TerminalFrame*    FRAME, 
    FramePos          POINT_A, 
    FramePos          POINT_B, 
    FramePos          POINT_C, 
    RGBcolor          COLORS[3])
{
  int ax = POINT_A.x; int ay = POINT_A.y; int az = POINT_A.z;
  int bx = POINT_B.x; int by = POINT_B.y; int bz = POINT_B.z;
  int cx = POINT_C.x; int cy = POINT_C.y; int cz = POINT_C.z;

  RGBcolor c0 = COLORS[0];
  RGBcolor c1 = COLORS[1];
  RGBcolor c2 = COLORS[2];

  // - - - Bubble sort vertices by Y (ay <= by <= cy) and keep colors synchronized
  if (ay > by)
  {
    int temp = ax; ax = bx; bx = temp;
    temp = ay;     ay = by; by = temp;
    temp = az;     az = bz; bz = temp;
    RGBcolor ct = c0; c0 = c1; c1 = ct;
  }
  if (ay > cy)
  {
    int temp = ax; ax = cx; cx = temp;
    temp = ay;     ay = cy; cy = temp;
    temp = az;     az = cz; cz = temp;
    RGBcolor ct = c0; c0 = c2; c2 = ct;
  }
  if (by > cy)
  {
    int temp = bx; bx = cx; cx = temp;
    temp = by;     by = cy; cy = temp;
    temp = bz;     bz = cz; cz = temp;
    RGBcolor ct = c1; c1 = c2; c2 = ct;
  }

  int height = cy - ay;
  if (height == 0) return; // Degenerate triangle

  // - - - Bottom half (ay to by)
  if (ay != by)
  {
    int segmentHeight = by - ay;
    for (int y = ay; y <= by; ++y)
    {
      int x1 = ax + ((cx - ax) * (y - ay)) / height;
      int z1 = az + ((cz - az) * (y - ay)) / height;
      int r1 = c0.r + ((c2.r - c0.r) * (y - ay)) / height;
      int g1 = c0.g + ((c2.g - c0.g) * (y - ay)) / height;
      int b1 = c0.b + ((c2.b - c0.b) * (y - ay)) / height;

      int x2 = ax + ((bx - ax) * (y - ay)) / segmentHeight;
      int z2 = az + ((bz - az) * (y - ay)) / segmentHeight;
      int r2 = c0.r + ((c1.r - c0.r) * (y - ay)) / segmentHeight;
      int g2 = c0.g + ((c1.g - c0.g) * (y - ay)) / segmentHeight;
      int b2 = c0.b + ((c1.b - c0.b) * (y - ay)) / segmentHeight;

      int minX, maxX, minZ, maxZ;
      int minR, maxR, minG, maxG, minB, maxB;

      if (x1 > x2)
      {
        minX = x2; minZ = z2; minR = r2; minG = g2; minB = b2;
        maxX = x1; maxZ = z1; maxR = r1; maxG = g1; maxB = b1;
      }
      else
      {
        minX = x1; minZ = z1; minR = r1; minG = g1; minB = b1;
        maxX = x2; maxZ = z2; maxR = r2; maxG = g2; maxB = b2;
      }

      int width = maxX - minX;
      for (int x = minX; x <= maxX; ++x)
      {
        int currentZ = minZ;
        int currR = minR, currG = minG, currB = minB;

        if (width > 0)
        {
          int tNum   = x - minX;
          currentZ   = minZ + ((maxZ - minZ) * tNum) / width;
          currR      = minR + ((maxR - minR) * tNum) / width;
          currG      = minG + ((maxG - minG) * tNum) / width;
          currB      = minB + ((maxB - minB) * tNum) / width;
        }

        FramePos pos = { .x = x, .y = y, .z = (uint16_t)currentZ };
        TerminalColor tc = terminalColorFromRGB((uint8_t)currR, (uint8_t)currG, (uint8_t)currB);
        frameSetPixel(FRAME, pos, tc);
      }
    }
  }

  // - - - Top half (by to cy)
  if (by != cy)
  {
    int segmentHeight = cy - by;
    for (int y = by + 1; y <= cy; ++y)
    {
      int x1 = ax + ((cx - ax) * (y - ay)) / height;
      int z1 = az + ((cz - az) * (y - ay)) / height;
      int r1 = c0.r + ((c2.r - c0.r) * (y - ay)) / height;
      int g1 = c0.g + ((c2.g - c0.g) * (y - ay)) / height;
      int b1 = c0.b + ((c2.b - c0.b) * (y - ay)) / height;

      int x2 = bx + ((cx - bx) * (y - by)) / segmentHeight;
      int z2 = bz + ((cz - bz) * (y - by)) / segmentHeight;
      int r2 = c1.r + ((c2.r - c1.r) * (y - by)) / segmentHeight;
      int g2 = c1.g + ((c2.g - c1.g) * (y - by)) / segmentHeight;
      int b2 = c1.b + ((c2.b - c1.b) * (y - by)) / segmentHeight;

      int minX, maxX, minZ, maxZ;
      int minR, maxR, minG, maxG, minB, maxB;

      if (x1 > x2)
      {
        minX = x2; minZ = z2; minR = r2; minG = g2; minB = b2;
        maxX = x1; maxZ = z1; maxR = r1; maxG = g1; maxB = b1;
      }
      else
      {
        minX = x1; minZ = z1; minR = r1; minG = g1; minB = b1;
        maxX = x2; maxZ = z2; maxR = r2; maxG = g2; maxB = b2;
      }

      int width = maxX - minX;
      for (int x = minX; x <= maxX; ++x)
      {
        int currentZ = minZ;
        int currR = minR, currG = minG, currB = minB;

        if (width > 0)
        {
          int tNum   = x - minX;
          currentZ   = minZ + ((maxZ - minZ) * tNum) / width;
          currR      = minR + ((maxR - minR) * tNum) / width;
          currG      = minG + ((maxG - minG) * tNum) / width;
          currB      = minB + ((maxB - minB) * tNum) / width;
        }

        FramePos pos = { .x = x, .y = y, .z = (uint16_t)currentZ };
        TerminalColor tc = terminalColorFromRGB((uint8_t)currR, (uint8_t)currG, (uint8_t)currB);
        frameSetPixel(FRAME, pos, tc);
      }
    }
  }
}

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
