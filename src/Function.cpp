#include "Function.h"
#include "CalculationMethods.h"

#include "VectorConversions.h"

#include <iostream>

using namespace Eigen;

std::vector<Variable> tmp;
std::vector<Variable>& Function::globalVariables = tmp;

Function::Function() 
    :name(), expressions(1)
{
    UpdateFunction();
}

void Function::UpdateFunction() 
{
    // expressions[0] = "-(2/15)*cos(u)*(3*cos(v) - 30*sin(u) + 90*cos(u)^4*sin(u) - 60*cos(u)^6*sin(u) + 5*cos(u)*cos(v)*sin(u))";
    // expressions.emplace_back("-(1/15)*sin(u)*(3*cos(v) - 3*cos(u)^2*cos(v) - 48*cos(u)^4*cos(v) + 48*cos(u)^6*cos(v) - 60*sin(u) + 5*cos(u)*cos(v)*sin(u) - 5*cos(u)^3*cos(v)*sin(u) - 80*cos(u)^5*cos(v)*sin(u) + 80*cos(u)^7*cos(v)*sin(u))");
    // expressions.emplace_back("(2/15)*(3 + 5*cos(u)*sin(u))*sin(v)");
    // expressions.emplace_back("sin(v)");

    expressions[0] = "sin(x) + cos(y) - z";
    expressions.emplace_back("x^2 - y^2 - z");

    input = std::vector<float>(globalVariables.size() + localVariables.size());
    
    exprtk::symbol_table<float> symbolTable;
    for (size_t i = 0; i < globalVariables.size(); i++)
    { 
        symbolTable.add_variable(
            std::string(1, globalVariables[i].symbol), 
            input[i]
        );
    } 
    for (size_t i = 0; i < localVariables.size(); i++)
    { 
        symbolTable.add_variable(
            std::string(1, localVariables[i].symbol), 
            input[i + globalVariables.size()]
        );
    }
    
    funcs.resize(expressions.size());
    exprtk::parser<float> parser;
    for (size_t i = 0; i < expressions.size(); i++) 
    {
        funcs[i] = exprtk::expression<float>();
        funcs[i].register_symbol_table(symbolTable);
        
        parser.compile(expressions[i], funcs[i]);
    }

    CalculateForm();
}

void Function::CalculateForm() 
{   
    CalculationMethods::GetVectors(*this);

    std::vector<Vector3f> vects = 
        ConvertTo::Positions(CalculationMethods::interCurve, 0, 1, 2);
    std::vector<Vector3f> colors = 
        ConvertTo::Colors(CalculationMethods::interCurve, 1);

    form2.UpdateCurveData(vects, colors);

    std::vector<Vector3f> vects2 = 
        ConvertTo::Positions(CalculationMethods::vectors, 0, 1, 2);
    std::vector<Vector3f> colors2 = 
        ConvertTo::Colors(CalculationMethods::vectors);

	form.UpdateMeshData(vects2, colors2, CalculationMethods::triangles);
}

void Function::Draw()
{
    form.Draw();
}

void Function::Draw2()
{
    form2.Draw();
}

VectorXf Function::Evaluate(VectorXf &inputVector) const
{
    input.assign(inputVector.data(), inputVector.data() + inputVector.size());

    VectorXf output(funcs.size());
    for (size_t k = 0; k < funcs.size(); k++)
    {
        output[k] = funcs[k].value();
    }
    return output;
}

