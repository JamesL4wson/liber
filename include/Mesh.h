#pragma once

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>

using namespace Eigen;

struct Vertex 
{
    Vector3f position;
    Vector3f normal;
    Vector3f color;
};

class Mesh
{
    public:
        virtual void UpdateMeshData(std::vector<Vector3f> vertices, std::vector<uint32_t> triangles) = 0;
        void Draw();
        
    protected:
        void GenBuffers();
        void UpdateBuffers();

        static std::vector<uint32_t> TrisFromVerts(int width, int length);
        
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        uint32_t vao{}, vbo{}, ebo{};
}; 