#include <cmath>
#include "DirectSampling.h"

int DirectSampling::size = 100;
float DirectSampling::stepSize = 0.1f;
bool DirectSampling::done = false;
std::vector<VectorXf> DirectSampling::vectors = {};

void DirectSampling::CalculateVectors(VectorXf (*func)(const VectorXf&))
{
    int inputDimension = 2;
    int inputCount = 2;
    vectors.reserve(std::powf((float)size, inputDimension));
    
    int arrayLength = 1;
    for (int i = 0; i < inputDimension; i++)
    {
        arrayLength *= size;
    }

    VectorXf inputVector(inputDimension);
    for (int i = 0; i < inputDimension; i++)
    {
        inputVector[i] = -5;
    }

    std::vector<int> inputOdometer(inputDimension);
    done = false;
    int k = 0;
    while (!done)
    {
        vectors[k] = func(inputVector);

        inputVector[0] += stepSize;
        inputOdometer[0]++;

        CheckDimension(inputVector, inputOdometer, 0);

        k++;
    }
}

void DirectSampling::CheckDimension(VectorXf inputVector, std::vector<int> &inputOdometer, int dimension)
{
    if (inputOdometer[dimension] > 5)
    {
        if (dimension == inputVector.size() - 1)
        {
            done = true;
            return;
        }

        inputVector[dimension] = -5;
        inputOdometer[dimension] = 0;

        inputVector[dimension + 1] += stepSize;
        inputOdometer[dimension + 1]++;

        CheckDimension(inputVector, inputOdometer, dimension + 1);
    }
}