#include <exprtk/exprtk.hpp>

typedef exprtk::symbol_table<double> symbol_table_t;
typedef exprtk::expression<double>   expression_t;
typedef exprtk::parser<double>       parser_t;

int main() 
{
   double x = 0;
   double y = 0;

   symbol_table_t symbol_table;
   expression_t   expression;
   parser_t       parser;

   std::string expression_string = "x^2 + y^2";

   symbol_table.add_variable("x",x);
   symbol_table.add_variable("y",y);

   expression.register_symbol_table(symbol_table);

   parser.compile(expression_string,expression);

   x = 1.0;
   y = 2.0;
   expression.value(); // 1 * 2 + 3

   x = 3.7;
   expression.value(); // 3.7 * 2 + 3

   y = -9.0;
   expression.value(); // 3.7 * -9 + 3

   int i = 0;
    for (x = 0.0; x < 10.0; x += 0.01)
    {
        for (y = 0.0; y < 10.0; y += 0.01)
        {
            expression.value();
        }
        i++;
        printf("%d\n", i);
    }
}