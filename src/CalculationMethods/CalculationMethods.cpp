#include "CalculationMethods.h"
#include "MarchingTable.h"

#include <Eigen/Core>
#include <iostream>
#include <cmath>
#include <chrono>

namespace CalculationMethods 
{
    Vector3f basis[3] = {{1,0,0},{0,1,0},{0,0,1}};
    VectorXf initalPosition = {};
    float stepSize = 0.1f;
    int size = 100;
    
    std::vector<VectorXf> vectors = {};
    std::vector<uint32_t> triangles = {};

    std::vector<VectorXf> interCurve = {};
    std::vector<VectorXf> interPoints = {};

    void GetVectors(const Function &function)
    {
        initalPosition.resize(function.globalVariables.size());
        for (size_t i = 0; i < function.globalVariables.size(); i++)
        {
            initalPosition[i] = function.globalVariables[i].min;
        }

        // MarchingCubes::CheckCube(function, 5.0f, initalPosition);
        MarchingCubes::NaiveMarch(function);
        MarchingCubes::FindInterCurve(function);
        // DirectSampling::CalculateVectors(function);
    }

    namespace MarchingCubes 
    {
        //Surface Extraction

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
                            
                            cubeCorners[i] = function.Evaluate(nSpaceCorner)[0];
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

                cubeCorners[0] = function.Evaluate(nSpaceCorner)[0];

                for (size_t i = 1; i < 8; i++)
                {
                    subSpaceCorner = subSpacePos + CubesTable::Corners[i] * currentStep;
                    nSpaceCorner = (
                        basis[0] * subSpaceCorner[0] + 
                        basis[1] * subSpaceCorner[1] + 
                        basis[2] * subSpaceCorner[2]);

                    cubeCorners[i] = function.Evaluate(nSpaceCorner)[0];

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
            // int outputDimension = 3;

            // VectorXf nSpaceCorner(outputDimension);
            // float cubePoints[27];

            // if (currentStep < stepSize) 
            // {
            //     for (int i = 0; i < 27; i++)
            //     {
            //         for (int k = 0; k < outputDimension; k++) 
            //         {
            //             nSpaceCorner[k] = (
            //                 basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
            //                 basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
            //                 basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
            //         }
            //         cubePoints[i] = function.Evaluate(nSpaceCorner)[0];
            //     }
                
            //     float cubeCorners[8];
            //     for (int i = 0; i < 8; i++)
            //     {
            //         for (int k = 0; k < 8; k++)
            //         {
            //             cubeCorners[k] = cubePoints[CubesTable::BinCubes[i][k]];
            //         }

            //         GetVertices(subSpaceCenter - (CubesTable::CentreToCorner[i] * currentStep), cubeCorners, currentStep);
            //     }
            //     return;
            // }

            // for (int k = 0; k < outputDimension; k++) 
            // {
            //     nSpaceCorner[k] = (
            //         basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[0][0] * currentStep) + 
            //         basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[0][1] * currentStep) +
            //         basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[0][2] * currentStep));
            // }
            // cubePoints[0] = function.Evaluate(nSpaceCorner)[0];

            // for (int i = 1; i < 27; i++)
            // {
            //     for (int k = 0; k < outputDimension; k++) 
            //     {
            //         nSpaceCorner[k] = (
            //             basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
            //             basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
            //             basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
            //     }
            //     cubePoints[i] = function.Evaluate(nSpaceCorner)[0];

            //     if (cubePoints[i] != cubePoints[0]) 
            //     {
            //         for (int k = 0; k < CubesTable::NumAdjCubes[i]; k++)
            //         {
            //             //check that sub-cube hasn't already been checked

            //             //move centre point
            //             CheckCube(function, currentStep/2, subSpaceCenter);
            //         }
            //     }
            // }
        }

        //Intersection Extraction

        void FindInterCurve(const Function &function)
        {
            for (size_t i = 0; i < triangles.size(); i += 3)
            {
                float triCorners[3];
                triCorners[0] = function.Evaluate(vectors[triangles[i + 0]])[1];
                triCorners[1] = function.Evaluate(vectors[triangles[i + 1]])[1];
                triCorners[2] = function.Evaluate(vectors[triangles[i + 2]])[1];

                int caseNumber = CubesTable::GetCase(triCorners, 3);

                if (caseNumber == 0 || caseNumber == 7) 
                {
                    continue;
                }

                Vector3f edgeStart = vectors[triangles[i + CubesTable::Verts[caseNumber][0]]];
                Vector3f edgeEnd = vectors[triangles[i + CubesTable::Verts[caseNumber][1]]];

                float t = (-triCorners[CubesTable::Verts[caseNumber][0]]
                            / (triCorners[CubesTable::Verts[caseNumber][1]] 
                                - triCorners[CubesTable::Verts[caseNumber][0]]));

                Eigen::Vector3f vertex(
                    edgeStart[0] + (edgeEnd[0] - edgeStart[0]) * t,
                    edgeStart[1] + (edgeEnd[1] - edgeStart[1]) * t,
                    edgeStart[2] + (edgeEnd[2] - edgeStart[2]) * t);
                
                interCurve.push_back(vertex);
            }
        }

        void FindInterPoint(const Function &function)
        {
            for (size_t i = 0; i < interCurve.size(); i += 2)
            {
                float vertices[2];
                vertices[0] = function.Evaluate(interCurve[i + 0])[2];
                vertices[1] = function.Evaluate(interCurve[i + 1])[2];

                if ((vertices[0] > 0) != (vertices[1] > 0))
                {
                    float t = -vertices[0] / (vertices[1] - vertices[0]);

                    Eigen::Vector3f vertex(
                        interCurve[i][0] + (interCurve[i + 1][0] - interCurve[i][0]) * t,
                        interCurve[i][1] + (interCurve[i + 1][1] - interCurve[i][1]) * t,
                        interCurve[i][2] + (interCurve[i + 1][2] - interCurve[i][2]) * t);

                    interPoints.push_back(vertex);
                }
            }
        }

        //Methods
        
        void GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, float currentStep) 
        {
            int caseNumber = CubesTable::GetCase(cubeCorners, 8);

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
            vectors.reserve(std::pow(size + 1, initalPosition.size()));
            
            VectorXf inputVector = initalPosition;
            VectorXi inputOdometer(inputVector.size());

            for (size_t k = 0; k < vectors.capacity(); k++)
            {
                VectorXf outputVector = function.Evaluate(inputVector);
                vectors.emplace_back(inputVector.size() + outputVector.size());
                vectors.back() << inputVector, outputVector;

                for (size_t i = 0; i < inputVector.size(); i++)
                {
                    inputVector[i] += stepSize;
                    inputOdometer[i]++;

                    if (inputOdometer[i] < size + 1) {break;}
                    else
                    {
                        inputVector[i] = initalPosition[i];
                        inputOdometer[i] = 0;
                    }
                }
            }
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
