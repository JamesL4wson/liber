#pragma once

#include "Mesh.h"

class Surface : public Mesh 
{
    public: 
        Surface();
        void UpdateMeshData(std::vector<Vector3f> vertPositions, std::vector<uint32_t> triangles); 
};