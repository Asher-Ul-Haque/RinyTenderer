#pragma once
#include <justLibrary.h>
#include <cglm/struct.h>
#include <rinyTenderer/renderer/_structs.h>

/**
 * @brief : Creates a new mesh
 * @param MESH : Which mesh to init
 * @param VERTEX_COUNT : How many vertices
 * @param TRIANGLE_COUNT : How many triangles
 */
JUST_API void meshCreate(TerminalMesh* MESH, size_t VERTEX_COUNT, size_t TRIANGLE_COUNT);

/**
 * @brief : Destroy a mesh
 * @param MESH : Which mesh to destroy
 */
JUST_API void meshDestroy(TerminalMesh* MESH);

/**
 * @brief : Draw a mesh in a frame
 * @param FRAME : Which frame to draw to
 * @param MESH : What mesh to draw
 * @param WIREFRAME : Want a wireframe or full render 
 */
JUST_API void frameDrawMesh(TerminalFrame* FRAME, const TerminalMesh* MESH, bool WIREFRAME, TerminalShadingMode SHADING);
