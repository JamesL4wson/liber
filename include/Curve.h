#include <glm/glm.hpp>
#include "Mesh.h"
#include "Camera.h"

class Curve : public Mesh
{
    public:
        Curve(Camera &mainCamera);
        void UpdateMeshData(std::vector<glm::vec3> vertices, std::vector<uint32_t> triangles);
        void SetLineWidth();
    private:
        void FindForwardVectors();
        std::vector<glm::vec3> forwardVectorsMain;
        Camera *mainCamera;
        float scalar = 1.5f;
};