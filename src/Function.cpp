#include "Function.h"

Function::Function(
    std::string name, 
    std::string exprString, 
    uint8_t inDim, 
    uint8_t outDim, 
    uint16_t typeCode) 

    :name(name), expr(exprString), inDim(inDim), outDim(outDim), typeCode(typeCode)
{
    parser<float> parser;

    inputs.resize(inDim);
    for (size_t i = 0; i < inDim; i++)
    {
        symbolTable.add_variable("x" + std::to_string(i), inputs[i]);
    }
    
    func.register_symbol_table(symbolTable);
    parser.compile(exprString, func);
};

void Function::UpdateFunction(std::string name, std::string newExpression) 
{
};

float Function::Evaluate(VectorXf inputVector) 
{
    for (size_t i = 0; i < inputs.size(); i++) 
    {
        inputs[i] = inputVector[i];
    }
    return func.value();
};