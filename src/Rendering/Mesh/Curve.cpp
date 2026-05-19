#include <glad/glad.h>
#include "Curve.h"

using namespace Eigen;

Curve::Curve() 
{
    GenBuffers();
}

void Curve::GenBuffers()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
		(void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1); 

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(2); 
}

void Curve::UpdateBuffers()
{
    glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
}

void Curve::Draw()
{
    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size());
    glBindVertexArray(0);
}

void Curve::UpdateCurveData(
        std::vector<Vector3f> vertPositions, 
        std::vector<Vector3f> colors)
{
    vertices.resize(vertPositions.size());
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].position = vertPositions[i];
    }
    
    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].color = colors[i];
    }

    for (size_t i = 0; i < vertPositions.size(); i++)
    {
        vertices[i].normal = (vertPositions[i + 1] - vertPositions[i]).normalized();
    }
    
    UpdateBuffers();
}
