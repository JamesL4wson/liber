#include <vector>
#include <Eigen/Core>

namespace ConvertTo
{
	std::vector<Eigen::Vector3f> Positions(const std::vector<Eigen::VectorXf> &inVectors, int x, int y, int z);
    std::vector<Eigen::Vector3f> Colors(const std::vector<Eigen::VectorXf> &inVectors, int h = -1, int l = -1);
}