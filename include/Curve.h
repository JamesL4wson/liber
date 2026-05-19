#pragma once

#include <vector>
#include <Eigen/Core>
#include <Mesh.h>

class Curve 
{
    public:
        Curve();
        void UpdateCurveData(
            std::vector<Eigen::Vector3f> vertPositions, 
            std::vector<Eigen::Vector3f> colors = {});
        
        void Draw();
    private:
        void GenBuffers();
        void UpdateBuffers();

        uint32_t vao{}, vbo{};

        std::vector<Vertex> vertices;      
};