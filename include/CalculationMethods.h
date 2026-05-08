#pragma once

#include <Eigen/Core>

#include "Function.h"
#include "MarchingTable.h"

using namespace Eigen;

class Function;

namespace CalculationMethods
{
    extern Vector3f basis[3];
    extern VectorXf initalPosition;
    extern float stepSize;
    extern int size;

    extern std::vector<VectorXf> vectors;
    extern std::vector<uint32_t> triangles;   
    
    void GetVectors(const Function &function);

    namespace MarchingCubes
    {
        void NaiveMarch(const Function &function);
        void CheckCube(const Function &function, float currentStep, Vector3f subSpacePos);
        void FastCheckCube(const Function &function, float currentStep, Vector3f subSpaceCenter);
        void GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, const float &currentStep);
    }

    namespace DirectSampling
    {
        void CalculateVectors(const Function &function);
        bool CheckDimension(VectorXf &inputVector, Vector3i &inputOdometer, int dimension);
    }

    namespace DiffEqSolver
    {
        void CalculateVectors(const Function &function, float t);
        VectorXf RK4Step(const Function &function, float stepSize, VectorXf position, float t);
    }
}