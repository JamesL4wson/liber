#include "Surface.h"

Surface::Surface() 
{
    GenBuffers();
}

void Surface::UpdateMeshData(std::vector<Vector3f> vertPositions, std::vector<uint32_t> triangles) 
{
    indices = triangles;

    std::vector<Vector3f> normals(vertPositions.size());
    normals = GetNormals(vertPositions, triangles);
    
    vertices.resize(vertPositions.size());
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].position = vertPositions[i];
        vertices[i].color = Vector3f(1.0f, 1.0f, 1.0f);
        vertices[i].normal = normals[i];
    }

    UpdateBuffers();
}
