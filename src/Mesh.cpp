#include <glad/glad.h>
#include "Mesh.h"

void Mesh::GenBuffers() 
{
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void Mesh::UpdateBuffers(std::vector<glm::vec3> verts, std::vector<uint32_t> triangles)
{
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uint32_t), indices.data(), GL_DYNAMIC_DRAW);
}
	
void Mesh::Draw() const
{
	glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

std::vector<uint32_t> Mesh::TrisFromVerts(int width, int length) 
{
	int vert = 0;
	int tris = 0;

	std::vector<uint32_t> indices(width * length * 6);

	for (int z = 0; z < length; z++)
	{
		for (int x = 0; x < width; x++)
		{
			indices[tris + 0] = vert + 0; 
			indices[tris + 1] = vert + width + 1; 
			indices[tris + 2] = vert + 1; 

			indices[tris + 3] = vert + 1; 
			indices[tris + 4] = vert + width + 1; 
			indices[tris + 5] = vert + width + 2; 

			vert++;
			tris += 6;
		}
		vert++; 
	}

	return indices;
}
 