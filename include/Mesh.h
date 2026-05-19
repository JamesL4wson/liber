#pragma once

#include <Eigen/Core>
#include <vector>

struct Vertex 
{
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    Eigen::Vector3f color;
};

class Mesh
{
    public:
        virtual void UpdateMeshData(
            std::vector<Eigen::Vector3f> vertices, 
            std::vector<Eigen::Vector3f> colors = {},
            std::vector<uint32_t> triangles = {}) = 0;
        void Draw();
        
        static std::vector<uint32_t> TrisFromVerts(int width, int length);
        protected:
        void GenBuffers();
        void UpdateBuffers();
        
        static std::vector<Eigen::Vector3f> GetNormals(const std::vector<Eigen::Vector3f> &vertices, const std::vector<uint32_t> &indices);
        
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t vao{}, vbo{}, ebo{};
}; 