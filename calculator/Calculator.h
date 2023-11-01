#include<iostream>
#include <string>
#include <fstream>
#include <stack>
#include <queue>

class Calculator
{
public:
    Calculator(const std::string& inputFileName, const std::string& outputFileName);
    void processInputFile();

private:
    int getOperatorPriority(char c);
    bool isOperator(char c);
    std::string remove_illegalchar(std::string expression);
    std::queue<std::string> in_to_post(std::string expression);
    double cal_post(std::queue<std::string> postfix);

    std::ifstream inputFile;
    std::ofstream outputFile;
};

Calculator::Calculator(const std::string& inputFileName, const std::string& outputFileName)
{
    inputFile.open(inputFileName);
    outputFile.open(outputFileName);

    if (!inputFile)
    {
        throw std::runtime_error("Unable to open input file");
    }
    if (!outputFile)
    {
        throw std::runtime_error("Unable to open output file");
    }
}

int Calculator::getOperatorPriority(char c) //Determine priority operator
{
    if (c == '+' || c == '-')
    {
        return 1;
    }
    else if (c == '*' || c == '/')
    {
        return 2;
    }
    else {
        return 0;
    }
}

bool Calculator::isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

std::string Calculator::remove_illegalchar(std::string expression) //Remove illegal characters
{
    std::string valid_exp;
    for (char c : expression)
    {
        if (isdigit(c) || c == '.' || isOperator(c) || c == '(' || c == ')')
        {
            valid_exp += c;
        }
    }
    return valid_exp;
}

std::queue<std::string> Calculator::in_to_post(std::string expression) //Convert infix expressions to posfix expressions
{
    std::stack<std::string> operators;
    std::queue<std::string> postfix;
    std::stack<std::string> parentheses;

    for (int i = 0; i < expression.length(); i++)
    {
        char c = expression[i];

        if (isdigit(c) || c == '.')
        {
            std::string num = "";
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.'))
            {
                num += expression[i];
                i++;
            }
            postfix.push(num);
            i--;
        }
        else if (isOperator(c))
        {
            while (!operators.empty() && getOperatorPriority(operators.top()[0]) >= getOperatorPriority(c))
            {
                postfix.push(operators.top());
                operators.pop();
            }
            operators.push(std::string(1, c));
        }
        else if (c == '(')
        {
            operators.push(std::string(1, c));
            parentheses.push(std::string(1, c)); // ½«×óÀ¨ºÅÈëÕ»
        }
        else if (c == ')')
        {
            while (!operators.empty() && operators.top() != "(")
            {
                postfix.push(operators.top());
                operators.pop();
            }
            if (!operators.empty() && operators.top() == "(")
            {
                operators.pop();
                parentheses.pop(); // Æ¥ÅäµÄ×óÀ¨ºÅ³öÕ»
            }
            else
            {
                throw std::runtime_error("Unmatched closing parenthesis.");
            }
        }
    }

    // ¼ì²éÊÇ·ñÓÐÎ´Æ¥ÅäµÄ×óÀ¨ºÅ
    while (!parentheses.empty())
    {
        throw std::runtime_error("Unmatched opening parenthesis.");
        parentheses.pop();
    }


    while (!operators.empty())
    {
        postfix.push(operators.top());
        operators.pop();
    }

    return postfix;
}

double Calculator::cal_post(std::queue<std::string> postfix) //Evaluate postfix expressions
{
    std::stack<double> operands; //Stack for numbers

    while (!postfix.empty())
    {
        std::string element = postfix.front();
        postfix.pop();

        if (isdigit(element[0]) || (element[0] == '-' && element.length() > 1 && isdigit(element[1])))
        {
            double num = std::stod(element);
            operands.push(num);
        }
        else if (isOperator(element[0]))
        {
            double num2 = operands.top();
            operands.pop();
            double num1 = operands.top();
            operands.pop();

            if (element == "+")
            {
                operands.push(num1 + num2);
            }
            else if (element == "-")
            {
                operands.push(num1 - num2);
            }
            else if (element == "*")
            {
                operands.push(num1 * num2);
            }
            else if (element == "/")
            {
                if (num2 == 0)
                {
                    throw std::runtime_error("Division by zero is not allowed.");
                }
                operands.push(num1 / num2);
            }
        }
    }

    if (!operands.empty())
    {
        return operands.top();
    }
    else
    {
        throw std::runtime_error("Error");
    }
}

void Calculator::processInputFile() //Read the inputfile to get expressions by line
{
    std::string line;
    while (std::getline(inputFile, line))
    {
        std::string valid_exp = remove_illegalchar(line);
        try
        {
            std::queue<std::string> postfix = in_to_post(valid_exp);
            double result = cal_post(postfix);

            outputFile << result << std::endl;
        }
        catch (const std::exception& e)
        {
            outputFile << "Error: " << e.what() << std::endl;
        }
    }
}