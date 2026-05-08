#include "CalculationMethods.h"
#include "MarchingTable.h"

#include <Eigen/Core>

namespace CalculationMethods 
{
    Vector3f basis[3] = {{1,0,0},{0,1,0},{0,0,1}};
    VectorXf initalPosition = {};
    float stepSize = 0.1f;
    int size = 100;
    
    std::vector<VectorXf> vectors = {};
    std::vector<uint32_t> triangles = {};   

    void GetVectors(const Function &function)
    {
        initalPosition.resize(3);
        initalPosition << -5, -5, -5;

        MarchingCubes::NaiveMarch(function);
    }

    namespace MarchingCubes 
    {
        void NaiveMarch(const Function &function)
        {
            Vector3f subSpacePos;
            Vector3f subSpaceCorner;
            VectorXf nSpaceCorner(3);

            float cubeCorners[8];
            for (size_t x = 0; x < size; x++)
            {
                subSpacePos[0] = x * stepSize;
                for (size_t y = 0; y < size; y++)
                {
                    subSpacePos[1] = y * stepSize;
                    for (size_t z = 0; z < size; z++)
                    {
                        subSpacePos[2] = z * stepSize;
                        for (size_t i = 0; i < 8; i++)
                        {
                            subSpaceCorner = subSpacePos + CubesTable::Corners[i] * stepSize;
                            nSpaceCorner = (
                                basis[0] * subSpaceCorner[0] + 
                                basis[1] * subSpaceCorner[1] + 
                                basis[2] * subSpaceCorner[2] +
                                initalPosition);
                            
                            cubeCorners[i] = function.Evaluate(nSpaceCorner);
                        }
                        
                        GetVertices(subSpacePos, cubeCorners, stepSize);
                    }
                }
            }
        }
    
        void CheckCube(const Function &function, float currentStep, Vector3f subSpacePos) 
        {
            VectorXf nSpaceCorner(basis[0].size());
            Vector3f subSpaceCorner;
            float cubeCorners[8];
            for (size_t k = 0; k < 8; k++)
            {
                bool cornersDiffer = false;
                subSpaceCorner = subSpacePos + CubesTable::Corners[0] * currentStep;
                nSpaceCorner = (
                    basis[0] * subSpaceCorner[0] + 
                    basis[1] * subSpaceCorner[1] + 
                    basis[2] * subSpaceCorner[2]);

                cubeCorners[0] = function.Evaluate(nSpaceCorner);

                for (size_t i = 1; i < 8; i++)
                {
                    subSpaceCorner = subSpacePos + CubesTable::Corners[i] * currentStep;
                    nSpaceCorner = (
                        basis[0] * subSpaceCorner[0] + 
                        basis[1] * subSpaceCorner[1] + 
                        basis[2] * subSpaceCorner[2]);

                    cubeCorners[i] = function.Evaluate(nSpaceCorner);

                    if ((cubeCorners[i] > 0) != (cubeCorners[0] > 0) || cornersDiffer)
                    {
                        cornersDiffer = true;
                        if (currentStep > stepSize)
                        {
                            CheckCube(function, currentStep/2, subSpacePos);
                            break;
                        }
                        else if (i == 7) 
                        {
                            GetVertices(subSpacePos, cubeCorners, currentStep);
                        }
                    }
                }

                subSpacePos[CubesTable::TraverseCube[k][0]] += CubesTable::TraverseCube[k][1] * currentStep;
            }
        }

        void FastCheckCube(const Function &function, float currentStep, Vector3f subSpaceCenter)
        {
            int outputDimension = 3;

            VectorXf nSpaceCorner(outputDimension);
            float cubePoints[27];

            if (currentStep < stepSize) 
            {
                for (int i = 0; i < 27; i++)
                {
                    for (int k = 0; k < outputDimension; k++) 
                    {
                        nSpaceCorner[k] = (
                            basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
                            basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
                            basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
                    }
                    cubePoints[i] = function.Evaluate(nSpaceCorner);
                }
                
                float cubeCorners[8];
                for (int i = 0; i < 8; i++)
                {
                    for (int k = 0; k < 8; k++)
                    {
                        cubeCorners[k] = cubePoints[CubesTable::BinCubes[i][k]];
                    }

                    GetVertices(subSpaceCenter - (CubesTable::CentreToCorner[i] * currentStep), cubeCorners, currentStep);
                }
                return;
            }

            for (int k = 0; k < outputDimension; k++) 
            {
                nSpaceCorner[k] = (
                    basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[0][0] * currentStep) + 
                    basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[0][1] * currentStep) +
                    basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[0][2] * currentStep));
            }
            cubePoints[0] = function.Evaluate(nSpaceCorner);

            for (int i = 1; i < 27; i++)
            {
                for (int k = 0; k < outputDimension; k++) 
                {
                    nSpaceCorner[k] = (
                        basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
                        basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
                        basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
                }
                cubePoints[i] = function.Evaluate(nSpaceCorner);

                if (cubePoints[i] != cubePoints[0]) 
                {
                    for (int k = 0; k < CubesTable::NumAdjCubes[i]; k++)
                    {
                        //check that sub-cube hasn't already been checked

                        //move centre point
                        CheckCube(function, currentStep/2, subSpaceCenter);
                    }
                }
            }
        }

        void GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, const float &currentStep) 
        {
            int caseNumber = CubesTable::GetCase(cubeCorners);

            if (caseNumber == 0 || caseNumber == 255)
            {
                return;
            }

            for (size_t edgeIndex = 0; edgeIndex < 15; edgeIndex++)
            {
                int triTableValue = CubesTable::Triangles[caseNumber][edgeIndex];

                if (triTableValue == -1) 
                {
                    return;
                }

                Vector3f edgeStart = subSpacePos + CubesTable::Edges[triTableValue][0] * currentStep;
                Vector3f edgeEnd = subSpacePos + CubesTable::Edges[triTableValue][1] * currentStep;

                float t = (-cubeCorners[CubesTable::EdgeToCorner[triTableValue][0]] 
                            / (cubeCorners[CubesTable::EdgeToCorner[triTableValue][1]]
                                - cubeCorners[CubesTable::EdgeToCorner[triTableValue][0]]));

                Eigen::Vector3f vertex(
                    edgeStart[0] + (edgeEnd[0] - edgeStart[0]) * t,
                    edgeStart[1] + (edgeEnd[1] - edgeStart[1]) * t,
                    edgeStart[2] + (edgeEnd[2] - edgeStart[2]) * t);

                vectors.push_back(vertex);
                triangles.push_back((uint32_t)(vectors.size() - 1));
            }
    }
    }

    namespace DirectSampling
    {
        void CalculateVectors(const Function &function)
        {
            int inputDimension = 2;
            vectors.reserve(std::powf((float)size, inputDimension));

            VectorXf inputVector = initalPosition;
            Vector3i inputOdometer(inputDimension);

            while (true)
            {
                VectorXf output(3);
                output << inputVector[0], inputVector[1], function.Evaluate(inputVector);
                vectors.push_back(output);

                inputVector[0] += stepSize;
                inputOdometer[0]++;

                if (CheckDimension(inputVector, inputOdometer, 0))
                {
                    break;
                }
            }
        }

        bool CheckDimension(VectorXf &inputVector, Vector3i &inputOdometer, int dimension)
        {
            if (inputOdometer[dimension] > 5)
            {
                if (dimension == inputVector.size() - 1)
                {
                    return false;
                }

                inputVector[dimension] = -5;
                inputOdometer[dimension] = 0;

                inputVector[dimension + 1] += stepSize;
                inputOdometer[dimension + 1]++;

                CheckDimension(inputVector, inputOdometer, dimension + 1);
            }

            return true; 
        }
    }

    namespace DiffEqSolver
    {
        void CalculateVectors(const Function &function, float t) 
        {
            std::vector<VectorXf> vectors(size);

            VectorXf position = initalPosition;
            for (int i = 0; i < size; i++)
            { 
                position = RK4Step(function, stepSize, position, t);
                vectors[i] = position;

                t += stepSize;
            }
        }

        VectorXf RK4Step(const Function &function, float stepSize, VectorXf position, float t) 
        {
            // VectorXf f1 = function(VectorXf(xk, tk));
            // VectorXf f2 = function(VectorXf(xk + (float)dt/2 * f1, t + (float)dt/2));
            // VectorXf f3 = function(VectorXf(xk + (float)dt/2 * f2, t + (float)dt/2));
            // VectorXf f4 = function(VectorXf(xk + (float)dt * f3, t + dt));

            // return xk + (float)dt/6 * (f1 + 2.0f * f2 + 2.0f * f3 + f4);
        }
    }
}
