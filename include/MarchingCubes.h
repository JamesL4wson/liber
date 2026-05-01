#pragma once

#include <vector>
#include <Eigen/Core>

#include "MarchingTable.h"
#include "Function.h"

using namespace Eigen;

class MarchingCubes 
{
    public:
        static void MarchCubes(Function &function, const VectorXf &initalValue);
        static void CheckCube(VectorXf (*func)(const VectorXf&), float currentStep, Vector3f subSpacePos);
        static void FastCheckCube(VectorXf (*func)(VectorXf&), float currentStep, Vector3f subSpacePos);

        static Vector3f *basis;
        static int outputDimension;
        static float stepSize;

        static uint32_t Samples;
         
        static std::vector<Vector3f> vertices;
        static std::vector<uint32_t> triangles;
    private:
        static void GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, const float &currentStep);

        static uint32_t size;
};