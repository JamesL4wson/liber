#include <glm/glm.hpp>
#include "Mesh.h"
#include "Camera.h"

class Curve : public Mesh
{
    public:
        Curve(Camera &mainCamera);
        void UpdateMeshData(std::vector<Vector3f> vertices, std::vector<uint32_t> triangles);
        void SetLineWidth();
    private:
        void FindForwardVectors();
        std::vector<Vector3f> forwardVectorsMain;

        Camera &mainCamera;
        float scalar = 1.5f;
};