#pragma once

#include <string>
#include <Eigen/Core>
#include <exprtk/exprtk.hpp>

#include "CalculationMethods.h"

#include "Mesh.h"
#include "Variable.h"
#include "Surface.h"

using namespace Eigen;

class Function 
{
    public:
        Function(std::string name, std::string exprString); 
        
        void UpdateFunction();
        void Draw();

        float Evaluate(VectorXf &inputVector) const;

        std::string name;
        std::string expr;

        std::vector<Variable> variables;
        
    private:
        void GetForm();

        exprtk::symbol_table<float> symbolTable;

        mutable std::vector<float> inputs;

        exprtk::expression<float> func;
        Surface form;
};