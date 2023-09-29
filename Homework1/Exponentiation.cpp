#include <iostream>
#include <cmath>

double power(double x, int n) 
{ 
    if (n == 0) 
        return 1;
    double p = power(x, n / 2);
    if (n % 2 == 0)
        return p * p;
    else
        return p * p * x;
}

int main()
{
    //Test
    double base;
    int exponent;
    std::cout << "Insert Base : ";
    std::cin >> base;
    std::cout << "Insert exponent : ";
    std::cin >> exponent;
    double t = power(base, exponent);
    std::cout << "Answer : " << t << std::endl;
    return 0;
}