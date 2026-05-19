#include "Surface.h"

using namespace Eigen;

Surface::Surface() 
{
    GenBuffers();
}

void Surface::UpdateMeshData(
    std::vector<Vector3f> vertPositions, 
    std::vector<Vector3f> colors, 
    std::vector<uint32_t> triangles) 
{
    //indices
    if (triangles.size() == 0) {indices = TrisFromVerts(100, 100);}
    else {indices = triangles;}
    
    //normals
    std::vector<Vector3f> normals = GetNormals(vertPositions, triangles);
    
    //add pos data
    vertices.resize(vertPositions.size());
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].position = vertPositions[i];
        vertices[i].normal = normals[i];
    }
    
    //add color data
    if (colors.size() != 0)
    {
        for (size_t i = 0; i < vertPositions.size(); i++)
        {
            vertices[i].color = colors[i];
        }
    }
    else 
    {
        for (size_t i = 0; i < vertPositions.size(); i++)
        {
            vertices[i].color = Vector3f(1,1,1);
        }
    }
    
    UpdateBuffers();
}
