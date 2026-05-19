#include <glad/glad.h>
#include <Eigen/Geometry>
#include "Mesh.h"

using namespace Eigen;

void Mesh::GenBuffers() 
{
	glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

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

void Mesh::UpdateBuffers()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
	
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

std::vector<Vector3f> Mesh::GetNormals(const std::vector<Vector3f> &vertices, const std::vector<uint32_t> &indices)
{
	std::vector<Vector3f> normals(vertices.size());
    for (size_t i = 0; i < vertices.size(); i++)
    {
        normals[i] = Vector3f(0.0f, 0.0f, 0.0f);
    }
    
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        Vector3f A = vertices[indices[i + 0]];
        Vector3f B = vertices[indices[i + 1]];
        Vector3f C = vertices[indices[i + 2]];

        Vector3f normal = (A - B).cross(A - C);

        normals[indices[i + 0]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }

    for (size_t i = 0; i < vertices.size(); i++) 
    {
        normals[i].normalize();
    }

	return normals;
}

std::vector<uint32_t> Mesh::TrisFromVerts(int width, int length) 
{
	std::vector<uint32_t> indices(width * length * 6);
	
	int vert = 0;
	int tri = 0;
	for (int z = 0; z < length; z++)
	{
		for (int x = 0; x < width; x++)
		{
			indices[tri + 0] = vert + 0; 
			indices[tri + 1] = vert + width + 1; 
			indices[tri + 2] = vert + 1; 
			
			indices[tri + 3] = vert + 1; 
			indices[tri + 4] = vert + width + 1; 
			indices[tri + 5] = vert + width + 2; 

			vert++;
			tri += 6;
		}
		vert++; 
	}

	return indices;
}
