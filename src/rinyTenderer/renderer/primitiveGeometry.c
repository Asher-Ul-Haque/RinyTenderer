#include <rinyTenderer/renderer/primitiveGeometry.h>

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
