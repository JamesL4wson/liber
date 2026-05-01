#include "Function.h"

class DiffEqSolver 
{
    public:
        std::vector<VectorXf> CalculateVectors(VectorXf (*func)(const VectorXf&), float t0, VectorXf initialPos);
    private:
        VectorXf RK4Step(VectorXf (*func)(const VectorXf&), float dt, VectorXf xk, float tk);
};