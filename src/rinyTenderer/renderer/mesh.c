#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"
#include "justLibrary.h"
#include <rinyTenderer/renderer/mesh.h>

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

JUST_API void frameBufferDrawMesh(FrameBuffer *FRAME, const TerminalMesh *MESH)
{
  JUST_ASSERT_DEBUG(FRAME != NULL);
  JUST_ASSERT_DEBUG(MESH  != NULL);
  JUST_ASSERT_DEBUG_MESSAGE(MESH->vertices != NULL, "[MESH] : Cannot draw a mesh with no vertices");

  mat4s modelMatrix = mesh3DGetModelMatrix(MESH);

  for (size_t i = 0; i < MESH->vertexCount; ++i)
  {
    vec3s localPos  = MESH->vertices[i].position;
    vec4s localPos4 = glms_vec4(localPos, 1.0f);
    vec4s worldPos4 = glms_mat4_mulv(modelMatrix, localPos4);

    TODO_COMMENT("Add camera / view transform, projection and rasterization");
    (void) worldPos4;
  }
}
