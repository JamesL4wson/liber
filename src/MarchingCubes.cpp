#include "MarchingCubes.h"
#include <iostream>
#include <cmath>
#include <exprtk/exprtk.hpp>

using namespace Eigen;
using namespace exprtk;

float MarchingCubes::stepSize = 0.1f;
uint32_t MarchingCubes::Samples = 0;
uint32_t MarchingCubes::size = 100;
int MarchingCubes::outputDimension = 3;
std::vector<Vector3f> MarchingCubes::vertices{};
std::vector<uint32_t> MarchingCubes::triangles{};
Vector3f *MarchingCubes::basis = nullptr;

void MarchingCubes::MarchCubes(Function &function, const VectorXf &initialValue)
{
    Vector3f subSpacePos;
    Vector3f subSpaceCorner;
    VectorXf nSpaceCorner(outputDimension);

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
                        initialValue);
                    
                    cubeCorners[i] = function.Evaluate(nSpaceCorner);
                }
                
                MarchingCubes::GetVertices(subSpacePos, cubeCorners, stepSize);
            }
        }
    }
}

void MarchingCubes::CheckCube(VectorXf (*func)(const VectorXf&), float currentStep, Vector3f subSpacePos) 
{
    VectorXf nSpaceCorner(outputDimension);
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

        cubeCorners[0] = func(nSpaceCorner)[0];

        for (size_t i = 1; i < 8; i++)
        {
            subSpaceCorner = subSpacePos + CubesTable::Corners[i] * currentStep;
            nSpaceCorner = (
                basis[0] * subSpaceCorner[0] + 
                basis[1] * subSpaceCorner[1] + 
                basis[2] * subSpaceCorner[2]);

            cubeCorners[i] = func(nSpaceCorner)[0];

            if ((cubeCorners[i] > 0) != (cubeCorners[0] > 0) || cornersDiffer)
            {
                cornersDiffer = true;
                if (currentStep > stepSize)
                {
                    CheckCube(func, currentStep/2, subSpacePos);
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

void MarchingCubes::FastCheckCube(VectorXf (*func)(VectorXf&), float currentStep, Vector3f subSpaceCenter)
{
    // Vect nSpaceCorner(outputDimension);
    // float cubePoints[27];

    // if (currentStep < stepSize) 
    // {
    //     for (int i = 0; i < 27; i++)
    //     {
    //         for (int k = 0; k < basis[0].dimension; k++) 
    //         {
    //             nSpaceCorner[k] = (
    //                 basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
    //                 basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
    //                 basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
    //         }
    //         cubePoints[i] = func(nSpaceCorner)[0];
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

    // for (int k = 0; k < basis[0].dimension; k++) 
    // {
    //     nSpaceCorner[k] = (
    //         basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[0][0] * currentStep) + 
    //         basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[0][1] * currentStep) +
    //         basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[0][2] * currentStep));
    // }
    // cubePoints[0] = func(nSpaceCorner)[0];

    // for (int i = 1; i < 27; i++)
    // {
    //     for (int k = 0; k < basis[0].dimension; k++) 
    //     {
    //         nSpaceCorner[k] = (
    //             basis[0][k] * (subSpaceCenter[0] + CubesTable::Corners[i][0] * currentStep) + 
    //             basis[1][k] * (subSpaceCenter[1] + CubesTable::Corners[i][1] * currentStep) +
    //             basis[2][k] * (subSpaceCenter[2] + CubesTable::Corners[i][2] * currentStep));
    //     }
    //     cubePoints[i] = func(nSpaceCorner)[0];

    //     if (cubePoints[i] != cubePoints[0]) 
    //     {
    //         for (int k = 0; k < CubesTable::NumAdjCubes[i]; k++)
    //         {
    //             //check that sub-cube hasn't already been checked

    //             //move centre point
    //             CheckCube(func, currentStep/2, subSpaceCenter);
    //         }
    //     }
    // }
}

void MarchingCubes::GetVertices(const Vector3f &subSpacePos, const float *cubeCorners, const float &currentStep) 
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

        vertices.push_back(vertex);
        triangles.push_back((uint32_t)(vertices.size() - 1));
    }
}
