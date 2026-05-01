#include "Mesh.h"

class Surface : public Mesh 
{
    public: 
        Surface();
        void UpdateMeshData(std::vector<glm::vec3> inVerts, std::vector<uint32_t> triangles); 
};