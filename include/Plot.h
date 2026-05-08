#pragma once

#include <Eigen/Core>
#include <variant>

#include "Function.h"
#include "Variable.h"

struct Plot
{
    Eigen::Vector3f basis[3] = {{1,0,0},{0,1,0},{0,0,1}};
    Eigen::VectorXf initalPosition = {};
    float stepSize = 0.1f;
    int size = 100;

    std::vector<Function> functions = {};
    std::vector<Variable> globalVariables = {};
};
