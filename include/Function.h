#pragma once

#include <string>
#include <Eigen/Core>
#include <exprtk/exprtk.hpp>

#include "Mesh.h"
#include "Surface.h"
#include "Curve.h"
#include "Variable.h"

class Function 
{
    public:
        Function(); 
        
        void UpdateFunction();
        void Draw();
        void Draw2();

        Eigen::VectorXf Evaluate(Eigen::VectorXf &inputVector) const;

        std::string name;
        std::vector<std::string> expressions;

        std::vector<Variable> localVariables;
        static std::vector<Variable> &globalVariables;
    private:
        void CalculateForm();
        
        mutable std::vector<float> input;
        
        std::vector<exprtk::expression<float>> funcs;
        Surface form;
        Curve form2;
};