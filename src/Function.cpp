#include "Function.h"

Function::Function(std::string name, std::string exprString) 
    :name(name), expr(exprString), form(Surface())
{
    UpdateFunction();
};

void Function::UpdateFunction() 
{
    exprtk::parser<float> parser;

    symbolTable = exprtk::symbol_table<float>();
    inputs = std::vector<float>(variables.size());

    for (size_t i = 0; i < variables.size(); i++)
    {
        symbolTable.add_variable(std::to_string(variables[i].symbol), inputs[i]);
    }

    func = exprtk::expression<float>();
    func.register_symbol_table(symbolTable);

    parser.compile(expr, func);

    GetForm();
}

void Function::GetForm() 
{   
	CalculationMethods::GetVectors(*this);

    std::vector<Vector3f> vects(CalculationMethods::vectors.size());
    for (size_t i = 0; i < CalculationMethods::vectors.size(); i++)
    {
        vects[i][0] = CalculationMethods::vectors[i][0];
        vects[i][1] = CalculationMethods::vectors[i][1];
        vects[i][2] = CalculationMethods::vectors[i][2];
    }

	form.UpdateMeshData(vects,  CalculationMethods::triangles);
}

void Function::Draw()
{
    form.Draw();
}

float Function::Evaluate(VectorXf &inputVector) const
{
    for (size_t i = 0; i < inputs.size(); i++) 
    {
        inputs[i] = inputVector[i];
    }
    return func.value();
}
