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

    if (i0 >= MESH->vertexCount || i1 >= MESH->vertexCount || i2 >= MESH->vertexCount) continue;

    // - - - Draw triangle if all 3 vertices are visible within the frustum
    if (projected[i0].visible && projected[i1].visible && projected[i2].visible)
    {
      TerminalColor pixelColor = terminalPixelFromColor('#', 255, 200, 255);

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

}
