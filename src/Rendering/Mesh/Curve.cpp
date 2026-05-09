#include <glad/glad.h>
#include "Curve.h"

Curve::Curve(Camera &mainCamera) 
    : mainCamera(mainCamera)
{
    GenBuffers();
}

void Curve::UpdateMeshData(std::vector<Vector3f> vertPositions, std::vector<uint32_t> triangles) 
{    
    indices = TrisFromVerts(vertices.size() - 1, 1);

    std::vector<Vector3f> normals(vertPositions.size());
    normals = GetNormals(vertPositions, indices);
    
    vertices.resize(vertPositions.size());
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].position = vertPositions[i];
        vertices[i].color = Vector3f(1.0f, 1.0f, 1.0f);
        vertices[i].normal = normals[i];
    }

    FindForwardVectors();
}

void Curve::SetLineWidth()
{
    std::vector<Vector3f> verticesRibbon(vertices.size() * 2);

    for (int i = 0; i < vertices.size(); i++)
    {
        float depth = (vertices[i].position - mainCamera.position).dot(mainCamera.forward);
        float width = scalar * depth / (float)800;

        Vector3f outVector = mainCamera.position - vertices[i].position;
        Vector3f side = (forwardVectorsMain[i].cross(outVector)).normalized() * width;

        verticesRibbon[i] = vertices[i].position - side;
        verticesRibbon[i + vertices.size()] = vertices[i].position + side;            
    }

    UpdateBuffers();
}

void Curve::FindForwardVectors()
{
    std::vector<Vector3f> forwardVectors(vertices.size());

    Vector3f forwardVector = (vertices[1].position - vertices[0].position).normalized();
    forwardVectors[0] = forwardVector;      

    for (int k = 1; k < vertices.size() - 1; k++)
    {
        Vector3f dir1 = vertices[k].position - vertices[k - 1].position;
        Vector3f dir2 = vertices[k + 1].position - vertices[k].position;
        forwardVector = (dir1 + dir2).normalized();
        forwardVectors[k] = forwardVector;
    }

    forwardVector = (vertices[vertices.size() - 1].position - vertices[vertices.size() - 2].position).normalized();       
    forwardVectors[vertices.size() - 1] = forwardVector;

    forwardVectorsMain = forwardVectors;
}
