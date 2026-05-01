#include <Eigen/Core>
#include <vector>

using namespace Eigen;

class DirectSampling 
{
    public:
        static void CalculateVectors(VectorXf (*func)(const VectorXf&));

        static int size;
        static float stepSize;

        static std::vector<VectorXf> vectors;
    private:
        static void CheckDimension(VectorXf inputVector, std::vector<int> &inputOdometer, int dimension);
        static bool done;
};