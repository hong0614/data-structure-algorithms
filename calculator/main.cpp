#include "Calculator.h"

int main() 
{

    Calculator calculator("input.txt", "output.txt");
    calculator.processInputFile();

    return 0;
}