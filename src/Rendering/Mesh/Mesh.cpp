#include <glad/glad.h>
#include "Mesh.h"

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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
}

void Mesh::Draw()
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
 