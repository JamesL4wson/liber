#pragma once

#include <Eigen/Core>


class CubesTable 
{
    public:
        static const Eigen::Vector3f Corners[8];
        static const Eigen::Vector3f Edges[12][2];
        static const int EdgeToCorner[12][2];
        static const int Triangles[256][16];

        static const int TraverseCube[8][2];

        static const int NumAdjCubes[26];
        static const int BinCubes[8][8];
        static const Eigen::Vector3f CentreToCorner[8];

        static int GetCase(const float *cornerValues);
};

