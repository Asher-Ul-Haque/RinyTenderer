#include "cglm/struct/mat4.h"
#include "cglm/struct/mat3.h"
#include "cglm/types-struct.h"
#include "justLibrary.h"
#include "rinyTenderer/renderer/_structs.h"
#include "rinyTenderer/renderer/camera.h"
#include "rinyTenderer/renderer/frame.h"
#include <rinyTenderer/renderer/mesh.h>
#include <stdint.h>

JUST_API void meshCreate(TerminalMesh* MESH, size_t VERTEX_COUNT, size_t TRIANGLE_COUNT)
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

JUST_API void meshDestroy(TerminalMesh* MESH)
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

typedef struct projectedVertex
{
  FramePos  pos;
  bool      visible;
} ProjectedVertex;

static void projectMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, const mat4s* MODEL_MATRIX, ProjectedVertex* PROJECTED_VERTICES)
{
  mat4s viewMatrix  = camera3DGetViewMatrix(&FRAME->camera);
  mat4s projMatrix  = camera3DGetProjectionMatrix(&FRAME->camera);

  // - - - MVP  = Proj * View * Model
  mat4s viewXmodel = glms_mat4_mul(viewMatrix, *MODEL_MATRIX);
  mat4s mvpMatrix  = glms_mat4_mul(projMatrix, viewXmodel);

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
      PROJECTED_VERTICES[i].visible = false;
      continue;
    }

    // - - - Perspective division -> NDC coords
    float ndcX = clipPos4.x / clipPos4.w;
    float ndcY = clipPos4.y / clipPos4.w;
    float ndcZ = clipPos4.z / clipPos4.w;

    // - - - Frustrum check
    if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f || ndcZ < -1.0f || ndcZ > 1.0f)
    {
      PROJECTED_VERTICES[i].visible = false;
      continue;
    }

    // - - - viewport mapping to frame pixel grid
    float screenX = (ndcX + 1.0f) * 0.5f * (float) FRAME->size.x;
    float screenY = (1.0f - ndcY) * 0.5f * (float) FRAME->size.y;

    // - - - Map normalized depth to uint16_t depth range
    float     normalizedZ = (ndcZ + 1.0f) * 0.5f;
    uint16_t  depth16     = normalizedZ * 65535.0f;

    PROJECTED_VERTICES[i].pos.x    = screenX;
    PROJECTED_VERTICES[i].pos.y    = screenY;
    PROJECTED_VERTICES[i].pos.z    = depth16;
    PROJECTED_VERTICES[i].visible  = true;
  }
}

static void flatShadeMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, mat4s* MODEL_MATRIX, TerminalTriangle* TRIANGLE, RGBcolor* COLORS)
{
  int i0 = TRIANGLE->indices[0];
  int i1 = TRIANGLE->indices[1];
  int i2 = TRIANGLE->indices[2];

  // - - - Flat shading
  vec4s p0World4 = glms_mat4_mulv(*MODEL_MATRIX, glms_vec4(MESH->vertices[i0].position, 1.0f));
  vec4s p1World4 = glms_mat4_mulv(*MODEL_MATRIX, glms_vec4(MESH->vertices[i1].position, 1.0f));
  vec4s p2World4 = glms_mat4_mulv(*MODEL_MATRIX, glms_vec4(MESH->vertices[i2].position, 1.0f));

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

  
  RGBcolor v0 = MESH->vertices[i0].color;
  RGBcolor v1 = MESH->vertices[i1].color;
  RGBcolor v2 = MESH->vertices[i2].color;

  int baseR = (v0.r + v1.r + v2.r) / 3;
  int baseG = (v0.g + v1.g + v2.g) / 3;
  int baseB = (v0.b + v1.b + v2.b) / 3;

  float lightR = 0.0f;
  float lightG = 0.0f;
  float lightB = 0.0f;

  // - - - Directional lights
  for (uint8_t j = 0; j < FRAME->directionalLightCount; ++j)
  {
    TerminalLight light     = FRAME->directionalLights[j];
    vec3s         lightDir  = glms_vec3_negate(light.direction);
    float         dot       = glms_vec3_dot(normal, lightDir);

    if (dot > 0.0f)
    {
      float diffuse = dot * light.intensity;
      lightR += (light.color.r / 255.0f) * diffuse;
      lightG += (light.color.g / 255.0f) * diffuse;
      lightB += (light.color.b / 255.0f) * diffuse;
    }
  }

  // - - - Point lights
  for (uint8_t j = 0; j < FRAME->pointLightCount; ++j)
  {
    TerminalLight light     = FRAME->pointLights[j];
    vec3s         lightVec  = glms_vec3_sub(light.position, triCenter);
    float         distance  = glms_vec3_norm(lightVec);
    vec3s         lightDir  = glms_vec3_scale(lightVec, 1.0f / (distance + 0.0001f));

    float dot = glms_vec3_dot(normal, lightDir);
    if (dot > 0.0f)
    {
      float attenuation = 1.0f / (1.0f + 0.1f * distance + 0.01f * (distance * distance));
      float diffuse     = dot * attenuation * light.intensity;
      
      lightR += (light.color.r / 255.0f) * diffuse;
      lightG += (light.color.g / 255.0f) * diffuse;
      lightB += (light.color.b / 255.0f) * diffuse;
    }
  }

  // - - - Cap light
  lightR = lightR > 1.0f ? 1.0f : lightR;
  lightG = lightG > 1.0f ? 1.0f : lightG;
  lightB = lightB > 1.0f ? 1.0f : lightB;

  // - - - Compute Ambient Floor
  float ambR = (FRAME->ambientColor.r / 255.0f) * FRAME->ambientIntensity;
  float ambG = (FRAME->ambientColor.g / 255.0f) * FRAME->ambientIntensity;
  float ambB = (FRAME->ambientColor.b / 255.0f) * FRAME->ambientIntensity;

  int totalRed   = (int)(baseR * (lightR + ambR));
  int totalGreen = (int)(baseG * (lightG + ambG));
  int totalBlue  = (int)(baseB * (lightB + ambB));

  uint8_t finalR = totalRed   > 255 ? 255 : (uint8_t)totalRed;
  uint8_t finalG = totalGreen > 255 ? 255 : (uint8_t)totalGreen;
  uint8_t finalB = totalBlue  > 255 ? 255 : (uint8_t)totalBlue;

  RGBcolor color = (RGBcolor) {.r = finalR, .g = finalG, .b = finalB };
  COLORS[0]      = color;
  COLORS[1]      = color;
  COLORS[2]      = color;
}

static void unlitShadeMesh(const TerminalMesh* MESH, const TerminalTriangle* TRIANGLE, RGBcolor* COLORS)
{
  RGBcolor c0 = MESH->vertices[TRIANGLE->indices[0]].color;
  RGBcolor c1 = MESH->vertices[TRIANGLE->indices[1]].color;
  RGBcolor c2 = MESH->vertices[TRIANGLE->indices[2]].color;

  int finalR = (c0.r + c1.r + c2.r) / 3.0f;
  int finalG = (c0.g + c1.g + c2.g) / 3.0f;
  int finalB = (c0.b + c1.b + c2.b) / 3.0f;

  RGBcolor color = (RGBcolor) { .r = finalR, .g = finalG, .b = finalB };
  COLORS[0] = color;
  COLORS[1] = color;
  COLORS[2] = color;
}

static void smoothShadeMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, mat4s* modelMatrix, const TerminalTriangle* TRIANGLE, RGBcolor* COLORS)
{
  uint16_t indices[3] = { TRIANGLE->indices[0], TRIANGLE->indices[1], TRIANGLE->indices[2] };

  for (int v = 0; v < 3; ++v)
  {
    uint16_t        idx   = indices[v];
    TerminalVertex  vert  = MESH->vertices[idx];

    // - - - Transform vertex position to world space
    vec4s worldPos4 = glms_mat4_mulv(*modelMatrix, glms_vec4(vert.position, 1.0f));
    vec3s worldPos  = {{ worldPos4.x, worldPos4.y, worldPos4.z }};

    // - - -.Transform vertex normal to world space
    mat3s model3       = glms_mat4_pick3(*modelMatrix);
    mat3s normalMatrix = glms_mat3_transpose(glms_mat3_inv(model3));
    vec3s normal       = glms_mat3_mulv(normalMatrix, vert.normal);
    normal             = glms_vec3_normalize(normal);

    RGBcolor baseColor = vert.color;

    float lightR = 0.0f;
    float lightG = 0.0f;
    float lightB = 0.0f;

    // - - - Directional lights
    for (uint8_t j = 0; j < FRAME->directionalLightCount; ++j)
    {
      TerminalLight light     = FRAME->directionalLights[j];
      vec3s         lightDir  = glms_vec3_negate(light.direction);
      float         dot       = glms_vec3_dot(normal, lightDir);

      if (dot > 0.0f)
      {
        float diffuse = dot * light.intensity;
        lightR += (light.color.r / 255.0f) * diffuse;
        lightG += (light.color.g / 255.0f) * diffuse;
        lightB += (light.color.b / 255.0f) * diffuse;
      }
    }

    // - - - Point lights
    for (uint8_t j = 0; j < FRAME->pointLightCount; ++j)
    {
      TerminalLight light     = FRAME->pointLights[j];
      vec3s         lightVec  = glms_vec3_sub(light.position, worldPos);
      float         distance  = glms_vec3_norm(lightVec);
      vec3s         lightDir  = glms_vec3_scale(lightVec, 1.0f / (distance + 0.0001f));

      float dot = glms_vec3_dot(normal, lightDir);
      if (dot > 0.0f)
      {
        float attenuation = 1.0f / (1.0f + 0.1f * distance + 0.01f * (distance * distance));
        float diffuse     = dot * attenuation * light.intensity;

        lightR += (light.color.r / 255.0f) * diffuse;
        lightG += (light.color.g / 255.0f) * diffuse;
        lightB += (light.color.b / 255.0f) * diffuse;
      }
    }

    // - - - Cap lighting multiplier at 1.0f
    lightR = lightR > 1.0f ? 1.0f : lightR;
    lightG = lightG > 1.0f ? 1.0f : lightG;
    lightB = lightB > 1.0f ? 1.0f : lightB;

    // - - - Clean Ambient Contribution
    float ambR = (FRAME->ambientColor.r / 255.0f) * FRAME->ambientIntensity;
    float ambG = (FRAME->ambientColor.g / 255.0f) * FRAME->ambientIntensity;
    float ambB = (FRAME->ambientColor.b / 255.0f) * FRAME->ambientIntensity;

    // - - - Final Color Combination
    int totalRed   = (int)(baseColor.r * (lightR + ambR));
    int totalGreen = (int)(baseColor.g * (lightG + ambG));
    int totalBlue  = (int)(baseColor.b * (lightB + ambB));

    COLORS[v] = (RGBcolor)
      {
        .r = totalRed   > 255 ? 255 : (uint8_t)totalRed,
        .g = totalGreen > 255 ? 255 : (uint8_t)totalGreen,
        .b = totalBlue  > 255 ? 255 : (uint8_t)totalBlue
      };
  }
}

JUST_API void frameDrawMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, bool WIREFRAME, TerminalShadingMode SHADING)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(MESH  != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(MESH->vertices != NULL, "[MESH] : Cannot draw a mesh with no vertices");
  JUST_ASSERT_DEBUG_MESSAGE(SHADING < SHADING_COUNT, "[MESH] : Cannot draw a mesh with invalid SHADING");

  // - - - project
  mat4s           modelMatrix = mesh3DGetModelMatrix(MESH);
  ProjectedVertex projected[MESH->vertexCount];
  projectMesh(FRAME, MESH, &modelMatrix, projected);

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

    RGBcolor colors[3];
    switch (SHADING)
    {
      case SHADING_UNLIT  : unlitShadeMesh(MESH, &(MESH->triangles[i]), colors);                        break;
      case SHADING_FLAT   : flatShadeMesh(FRAME, MESH, &modelMatrix, &(MESH->triangles[i]), colors);    break;
      case SHADING_SMOOTH : smoothShadeMesh(FRAME, MESH, &modelMatrix, &(MESH->triangles[i]), colors);  break;
      default : return;
    }

    if (WIREFRAME)
    {
      frameLine(FRAME, projected[i0].pos, projected[i1].pos, colors[0], colors[1]);
      frameLine(FRAME, projected[i1].pos, projected[i2].pos, colors[1], colors[2]);
      frameLine(FRAME, projected[i2].pos, projected[i0].pos, colors[2], colors[0]);
    }
    else
    {
      frameTriangle(FRAME, projected[i0].pos, projected[i1].pos, projected[i2].pos, colors);
    }
  }
}
