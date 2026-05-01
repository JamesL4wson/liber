#pragma once

#include <string>
#include <Eigen/Core>
#include <exprtk/exprtk.hpp>
#include "Mesh.h"

using namespace Eigen;
using namespace exprtk;

class Function 
{
    public:
        Function(
            std::string name, 
            std::string exprString, 
            uint8_t inDim, 
            uint8_t outDim, 
            uint16_t typeCode); 
        void UpdateFunction(std::string name, std::string newExpression);
        float Evaluate(VectorXf inputVector);

        std::string name;
        std::string expr;
        uint16_t typeCode;
    private:
        std::vector<float> inputs;
        symbol_table<float> symbolTable;
        expression<float> func;

        uint8_t inDim;
        uint8_t outDim;
};