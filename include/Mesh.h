#pragma once

#include <glm/glm.hpp>
#include <vector>

class Mesh
{
    public:
        virtual void UpdateMeshData(std::vector<glm::vec3> vertices, std::vector<uint32_t> triangles) = 0;
        void UpdateBuffers(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices);
        void Draw() const;

    protected:
        void GenBuffers();
        static std::vector<uint32_t> TrisFromVerts(int width, int length);

        std::vector<glm::vec3> vertices;
        std::vector<uint32_t> indices;
        uint32_t vao{}, vbo{}, ebo{};
}; 