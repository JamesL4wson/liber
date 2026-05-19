#include "VectorConversions.h"
#include "ColorConversions.h"

using namespace Eigen;

namespace ConvertTo
{
    std::vector<Vector3f> Positions(
        const std::vector<VectorXf> &inVectors, 
        int x, int y, int z)
    {
        std::vector<Vector3f> outVertices;
        outVertices.reserve(inVectors.size());
        for (size_t i = 0; i < inVectors.size(); i++) 
        {
            outVertices.emplace_back(inVectors[i][x], inVectors[i][y], inVectors[i][z]);
        }
        return outVertices;
    }

    std::vector<Vector3f> Colors(
        const std::vector<VectorXf> &inVectors, 
        int h, int l)
    {
        std::vector<Vector3f> outColors;
        outColors.reserve(inVectors.size());

        if (h == -1)
        {
            for (size_t i = 0; i < inVectors.size(); i++) 
            {
                outColors.emplace_back(1.0f, 1.0f, 1.0f);
            }
            return outColors;
        }

        if (l == -1)
        {
            for (size_t i = 0; i < inVectors.size(); i++) 
            {
                Vector3f col(inVectors[i][h] * 360, 1, 1);
                col = hsv2rgb(col);
                outColors.push_back(col);
            }
        }
        else
        {
            for (size_t i = 0; i < inVectors.size(); i++) 
            {
                Vector3f col(inVectors[i][h], 1, inVectors[i][l]);
                col = hsv2rgb(col);
                outColors.push_back(col);
            }
        }
        return outColors;
    }

}
