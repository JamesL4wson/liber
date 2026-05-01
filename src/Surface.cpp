#include "Surface.h"

Surface::Surface() 
{
    GenBuffers();
}

void Surface::UpdateMeshData(std::vector<glm::vec3> inVerts, std::vector<uint32_t> triangles) 
{
    vertices = inVerts;
    indices = triangles;

    UpdateBuffers(vertices, indices);
}