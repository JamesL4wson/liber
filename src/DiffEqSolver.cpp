#include "DiffEqSolver.h"

std::vector<VectorXf> DiffEqSolver::CalculateVectors(VectorXf (*func)(const VectorXf&), float t0, VectorXf initialPos) 
{
    int size = 100;
    float stepSize = 0.1f;

    std::vector<VectorXf> vectors(size);

    float time = t0;
    VectorXf position = initialPos;
    for (int i = 0; i < size; i++)
    { 
        position = RK4Step(func, stepSize, position, time);
        vectors[i] = position;

        time += stepSize;
    }

    return vectors;
}

VectorXf DiffEqSolver::RK4Step(VectorXf (*func)(const VectorXf&), float dt, VectorXf xk, float tk) 
{
    // VectorXf f1 = func(VectorXf(xk, tk));
    // VectorXf f2 = func(VectorXf(xk + (float)dt/2 * f1, tk + (float)dt/2));
    // VectorXf f3 = func(VectorXf(xk + (float)dt/2 * f2, tk + (float)dt/2));
    // VectorXf f4 = func(VectorXf(xk + (float)dt * f3, tk + dt));

    // return xk + (float)dt/6 * (f1 + 2.0f * f2 + 2.0f * f3 + f4);
}