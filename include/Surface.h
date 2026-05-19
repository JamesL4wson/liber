#pragma once

#include "Mesh.h"

class Surface : public Mesh
{
    public: 
        Surface();
        void UpdateMeshData(
            std::vector<Eigen::Vector3f> vertPositions, 
            std::vector<Eigen::Vector3f> colors = {},
            std::vector<uint32_t> triangles = {}); 
};