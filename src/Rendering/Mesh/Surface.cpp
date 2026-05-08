#include "Surface.h"

Surface::Surface() 
{
    GenBuffers();
}

void Surface::UpdateMeshData(std::vector<Vector3f> vertPositions, std::vector<uint32_t> triangles) 
{
    indices = triangles;
    
    vertices.resize(vertPositions.size());
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].position = vertPositions[i];
        vertices[i].color = Vector3f(1.0f, 1.0f, 1.0f);

        vertices[i].normal = Vector3f(0.0f, 0.0f, 0.0f);
    }
    
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Vector3f A = vertPositions[indices[i + 0]];
        Vector3f B = vertPositions[indices[i + 1]];
        Vector3f C = vertPositions[indices[i + 2]];

        Vector3f normal = (A - B).cross(A - C);

        vertices[indices[i + 0]].normal += normal;
        vertices[indices[i + 1]].normal += normal;
        vertices[indices[i + 2]].normal += normal;
    }

    for (size_t i = 0; i < vertices.size(); i++) 
    {
        vertices[i].normal.normalize();
    }

    UpdateBuffers();
}
