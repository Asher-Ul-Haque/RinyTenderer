#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"
#include "justLibrary.h"
#include "rinyTenderer/renderer/camera.h"
#include "rinyTenderer/renderer/frame.h"
#include <rinyTenderer/renderer/mesh.h>
#include <stdint.h>

JUST_API void mesh3DCreate(TerminalMesh* MESH, size_t VERTEX_COUNT, size_t TRIANGLE_COUNT)
{
  JUST_LOG_DEBUG("[MESH] : Creating new 3D mesh (Vertices : %zu, Triangles: %zu)", VERTEX_COUNT, TRIANGLE_COUNT);
  JUST_ASSERT_DEBUG_MESSAGE(MESH != NULL, "[MESH] : Cannot initialize a NULL mesh");

  MESH->vertexCount   = VERTEX_COUNT;
  MESH->triangleCount = TRIANGLE_COUNT;

  // - - - Allocate vertices and triangles and using engine's tagged memory tracking
  size_t vertexBytes    = VERTEX_COUNT    * sizeof(TerminalVertex);
  size_t triangleBytes  = TRIANGLE_COUNT  * sizeof(TerminalTriangle);

  MESH->vertices = (TerminalVertex*) JUST_MALLOC_TAGGED(vertexBytes, "TERM_MESH");
  JUST_ASSERT_DEBUG_MESSAGE(MESH->vertices != NULL, "[MESH] : Vertex allocation failed");

  MESH->triangles = (TerminalTriangle*) JUST_MALLOC_TAGGED(triangleBytes, "TERM_MESH");
  JUST_ASSERT_DEBUG_MESSAGE(MESH->triangles != NULL, "[MESH] : Triangle allocation failed");

  // - - - Initialize default transform : Pos (0, 0, 0), Rot (0, 0, 0), Scale (1,1, 1)
  MESH->transform.position = (vec3s) {{0.0f, 0.0f, 0.0f}};
  MESH->transform.rotation = (vec3s) {{0.0f, 0.0f, 0.0f}};
  MESH->transform.scale    = (vec3s) {{1.0f, 1.0f, 1.0f}};

  JUST_LOG_INFO("[MESH] : 3D mesh successfully created");
}

JUST_API void mesh3DDestroy(TerminalMesh* MESH)
{
  JUST_LOG_DEBUG("[MESH] : Destroying 3D mesh");
  JUST_ASSERT_DEBUG_MESSAGE(MESH != NULL, "[MESH] : Cannot destroy a NULL MESH");

  JUST_FREE(MESH->vertices);
  JUST_FREE(MESH->triangles);

  MESH->vertices      = NULL;
  MESH->triangles     = NULL;
  MESH->vertexCount   = 0;
  MESH->triangleCount = 0;

  JUST_LOG_INFO("[MESH] : 3D mesh destroyed");
}

JUST_LOCAL static inline mat4s mesh3DGetModelMatrix(const TerminalMesh* MESH)
{
  mat4s model = glms_mat4_identity();

  // - - - Translate
  model = glms_translate(model, MESH->transform.position);

  // - - - Rotate
  model = glms_rotate(model, MESH->transform.rotation.x, (vec3s) {{ 1.0f, 0.0f, 0.0f }});
  model = glms_rotate(model, MESH->transform.rotation.y, (vec3s) {{ 0.0f, 1.0f, 0.0f }});
  model = glms_rotate(model, MESH->transform.rotation.z, (vec3s) {{ 0.0f, 0.0f, 1.0f }});

  // - - - Scale
  model = glms_scale(model, MESH->transform.scale);

  return model;
}

JUST_API void frameDrawMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, bool WIREFRAME)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(MESH  != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(MESH->vertices != NULL, "[MESH] : Cannot draw a mesh with no vertices");

  mat4s modelMatrix = mesh3DGetModelMatrix(MESH);  
  mat4s viewMatrix  = camera3DGetViewMatrix(&FRAME->camera);
  mat4s projMatrix  = camera3DGetProjectionMatrix(&FRAME->camera);

  // - - - MVP  = Proj * View * Model
  mat4s viewXmodel = glms_mat4_mul(viewMatrix, modelMatrix);
  mat4s mvpMatrix  = glms_mat4_mul(projMatrix, viewXmodel);

  typedef struct projectedVertex
  {
    FramePos  pos;
    bool      visible;
  } ProjectedVertex;

  ProjectedVertex projected[MESH->vertexCount];

  // - - - Transform and project all vertices
  for (size_t i = 0; i < MESH->vertexCount; ++i)
  {
    vec3s localPos  = MESH->vertices[i].position;
    vec4s localPos4 = glms_vec4(localPos, 1.0f);
    vec4s clipPos4  = glms_mat4_mulv(mvpMatrix, localPos4);

    // - - - Near plane clipping check (behind camera or w <= 0)
    if (clipPos4.w <= 0.0f)
    {
      JUST_LOG_DEBUG("[VERTEX %zu] CULLED: w <= 0 (w = %.2f)", i, clipPos4.w);
      projected[i].visible = false;
      continue;
    }

    // - - - Perspective division -> NDC coords
    float ndcX = clipPos4.x / clipPos4.w;
    float ndcY = clipPos4.y / clipPos4.w;
    float ndcZ = clipPos4.z / clipPos4.w;

    // - - - Frustrum check
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f || ndcZ < -1.0f || ndcZ > 1.0f)
    {
      projected[i].visible = false;
      continue;
    }

    // - - - viewport mapping to frame pixel grid
    float screenX = (ndcX + 1.0f) * 0.5f * (float) FRAME->size.cols;
    float screenY = (1.0f - ndcY) * 0.5f * (float) FRAME->size.rows;

    // - - - Map normalized depth to uint16_t depth range
    float     normalizedZ = (ndcZ + 1.0f) * 0.5f;
    uint16_t  depth16     = normalizedZ * 65535.0f;

    projected[i].pos.x    = screenX;
    projected[i].pos.y    = screenY;
    projected[i].pos.z    = depth16;
    projected[i].visible  = true;
  }

  // - - - Rasterize mesh triangles into the depth buffer
  for (size_t i = 0; i < MESH->triangleCount; ++i)
  {
    TerminalTriangle tri = MESH->triangles[i];

    uint16_t i0 = tri.indices[0];
    uint16_t i1 = tri.indices[1];
    uint16_t i2 = tri.indices[2];

    if (i0 >= MESH->vertexCount) continue;
    if (i1 >= MESH->vertexCount) continue;
    if (i2 >= MESH->vertexCount) continue;

    // - - - Draw triangle if all 3 vertices are visible within the frustum
    if (!projected[i0].visible) continue;
    if (!projected[i1].visible) continue; 
    if (!projected[i2].visible) continue;

    // - - - Backface culling
    int x0 = projected[i0].pos.x;
    int y0 = projected[i0].pos.y;
    int x1 = projected[i1].pos.x;
    int y1 = projected[i1].pos.y;
    int x2 = projected[i2].pos.x;
    int y2 = projected[i2].pos.y;

    int32_t crossProduct = (x1 - x0) * (y2 - y0) - (y1 - y0) * (x2 - x0);
    if (crossProduct > 0) continue;

    // - - - Flat shading
    vec4s p0World4 = glms_mat4_mulv(modelMatrix, glms_vec4(MESH->vertices[i0].position, 1.0f));
    vec4s p1World4 = glms_mat4_mulv(modelMatrix, glms_vec4(MESH->vertices[i1].position, 1.0f));
    vec4s p2World4 = glms_mat4_mulv(modelMatrix, glms_vec4(MESH->vertices[i2].position, 1.0f));

    vec3s p0World = {{ p0World4.x, p0World4.y, p0World4.z }};
    vec3s p1World = {{ p1World4.x, p1World4.y, p1World4.z }};
    vec3s p2World = {{ p2World4.x, p2World4.y, p2World4.z }};

    vec3s edge1   = glms_vec3_sub(p1World, p0World);
    vec3s edge2   = glms_vec3_sub(p2World, p0World);
    vec3s normal  = glms_vec3_normalize(glms_vec3_cross(edge1, edge2));

    vec3s triCenter = 
    {{ 
       (p0World.x + p1World.x + p2World.x) / 3.0f, 
       (p0World.y + p1World.y + p2World.y) / 3.0f, 
       (p0World.z + p1World.z + p2World.z) / 3.0f 
    }};

    RGBcolor  base      = MESH->vertices[i0].color;
    int       totalRed  = base.r * FRAME->ambientColor.r * FRAME->ambientIntensity;
    int       totalGreen = base.g * FRAME->ambientColor.g * FRAME->ambientIntensity;
    int       totalBlue = base.b * FRAME->ambientColor.b * FRAME->ambientIntensity;

    int factor = 255 * 255 * 255;
    for (uint8_t i = 0; i < FRAME->directionalLightCount; ++i)
    {
      TerminalLight light = FRAME->directionalLights[i];
      int diffuse = 0;

      vec3s lightDir  = glms_vec3_negate(light.direction);
      float dot       = glms_vec3_dot(normal, lightDir);
      if (dot > 0.0f) diffuse = (int) (dot * 255);


      totalRed    += (base.r * light.color.r * light.intensity * diffuse) / factor;
      totalGreen  += (base.g * light.color.g * light.intensity * diffuse) / factor;
      totalBlue   += (base.b * light.color.b * light.intensity * diffuse) / factor;
    }

    for (uint8_t i = 0; i < FRAME->pointLightCount; ++i)
    {
      TerminalLight light = FRAME->pointLights[i];
      int diffuse = 0;

      vec3s lightVec = glms_vec3_sub(light.position, triCenter);
      float distance = glms_vec3_norm(lightVec);
      vec3s lightDir = glms_vec3_scale(lightVec, 1.0f / (distance + 0.0001f));

      float dot = glms_vec3_dot(normal, lightDir);
      if (dot > 0.0f)
      {
        float attenuation = 1.0f / (1.0f + 0.1f * distance + 0.01f * (distance * distance));
        diffuse = dot * attenuation * 255;
      }

      totalRed    += (base.r * light.color.r * light.intensity * diffuse) / factor;
      totalGreen  += (base.g * light.color.g * light.intensity * diffuse) / factor;
      totalBlue   += (base.b * light.color.b * light.intensity * diffuse) / factor;
    }

    uint8_t finalR = totalRed   > 255 ? 255 : (uint8_t) totalRed;
    uint8_t finalG = totalGreen > 255 ? 255 : (uint8_t) totalGreen;
    uint8_t finalB = totalBlue  > 255 ? 255 : (uint8_t) totalBlue;

    TerminalColor pixelColor = terminalColorFromRGB(finalR, finalG, finalB);

    if (WIREFRAME)
    {
      frameLine(
          FRAME,
          projected[i0].pos,
          projected[i1].pos,
          pixelColor);
      frameLine(
          FRAME,
          projected[i1].pos,
          projected[i2].pos,
          pixelColor);
      frameLine(
          FRAME,
          projected[i2].pos,
          projected[i0].pos,
          pixelColor);
    }
    else
    {
      frameTriangle(
          FRAME,
          projected[i0].pos,
          projected[i1].pos,
          projected[i2].pos,
          pixelColor);
    }
  }
}
