#include <glad/glad.h>
#include "Curve.h"

Curve::Curve(Camera &mainCamera) 
    : mainCamera(&mainCamera)
{
    GenBuffers();
}

void Curve::UpdateMeshData(std::vector<glm::vec3> inVerts, std::vector<uint32_t> triangles) 
{
    vertices = inVerts;
    indices = TrisFromVerts(vertices.size() - 1, 1);
    
    FindForwardVectors();
}

void Curve::SetLineWidth()
{
    std::vector<glm::vec3> verticesRibbon(vertices.size() * 2);

    for (int i = 0; i < vertices.size(); i++)
    {
        float depth = glm::dot(vertices[i] - mainCamera->position, mainCamera->forward);
        float width = scalar * depth / (float)800;

        glm::vec3 outVector = mainCamera->position - vertices[i];
        glm::vec3 side = glm::normalize(glm::cross(forwardVectorsMain[i], outVector)) * width;

        verticesRibbon[i] = vertices[i] - side;
        verticesRibbon[i + vertices.size()] = vertices[i] + side;            
    }

    UpdateBuffers(verticesRibbon, indices);
}

void Curve::FindForwardVectors()
{
    std::vector<glm::vec3> forwardVectors(vertices.size());

    glm::vec3 forwardVector = glm::normalize(vertices[1] - vertices[0]);
    forwardVectors[0] = forwardVector;      

    for (int k = 1; k < vertices.size() - 1; k++)
    {
        glm::vec3 dir1 = vertices[k] - vertices[k - 1];
        glm::vec3 dir2 = vertices[k + 1] - vertices[k];
        forwardVector = glm::normalize(dir1 + dir2);
        forwardVectors[k] = forwardVector;
    }

    forwardVector = glm::normalize(vertices[vertices.size() - 1] - vertices[vertices.size() - 2]);       
    forwardVectors[vertices.size() - 1] = forwardVector;

    forwardVectorsMain = forwardVectors;
}
