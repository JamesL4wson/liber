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

    extern std::vector<VectorXf> interCurve;
    extern std::vector<VectorXf> interPoints;
    
    void GetVectors(const Function &function);

    namespace MarchingCubes
    {
        /*
            Surface Extraction

            Impliments the traditional marching cubes algorithm with fixed stepsize. This method is slow to run but produces the strongest 
            guarantee of topological accuracy. Suitable for manifolds that may contain fine holes or small, disperate, geometry.
        */
        void NaiveMarch(const Function &function);
        /*
            Surface Extraction

            Impliments the an iterative marching cubes algorithm that uses oct-trees to locate topology and efficiently skip empty voxels.
            
            Faster than NaiveMarch() but is more susceptible to incorrect results. If the object sampled may contain fine, or disperate 
            detail, this algorithm may fail to represent this. In this case NaiveMarch() may be more appropriate.
        */
        void CheckCube(const Function &function, float currentStep, Vector3f subSpacePos);
        /*
            Surface Extraction

            Applies same algorithm as CheckCube() method but avoids repeated work that CheckCube() can incur.
        */
        void FastCheckCube(const Function &function, float currentStep, Vector3f subSpaceCenter);
        void FindInterCurve(const Function &function);
        void FindInterPoint(const Function &function);
        void GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, float currentStep);
    }

    namespace DirectSampling
    {
        void CalculateVectors(const Function &function);
    }

    namespace DiffEqSolver
    {
        void CalculateVectors(const Function &function, float t);
        VectorXf RK4Step(const Function &function, float stepSize, VectorXf position, float t);
    }
}