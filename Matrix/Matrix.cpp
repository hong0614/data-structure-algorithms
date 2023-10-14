#include<iostream>
#include<vector>
#include"Matrix.h"

int main()
{
    Matrix<double> mat1(2, 2);
    mat1(0, 0) = 1;
    mat1(0, 1) = 2;
    mat1(1, 0) = 3;
    mat1(1, 1) = 4;

    Matrix<double> mat2(2, 2);
    mat2(0, 0) = 5;
    mat2(0, 1) = 6;
    mat2(1, 0) = 7;
    mat2(1, 1) = 8;
    
    cout << "Determinant of mat 1 is " << mat1.det() << endl;
    Matrix<double> result_add = mat1 + mat2;
    for (int i = 0; i < result_add.rows(); i++)
    {
        for (int j = 0; j < result_add.cols(); j++)
        {
            cout << result_add(i, j) << " ";
        }
        std::cout << std::endl;
    }

    Matrix<double> mat3(3, 3);
    mat3(0, 0) = 1;
    mat3(0, 1) = 2;
    mat3(0, 2) = 3;
    mat3(1, 0) = 4;
    mat3(1, 1) = 5;
    mat3(1, 2) = 6;
    mat3(2, 0) = 4;
    mat3(2, 1) = 5;
    mat3(2, 2) = 7;
    
    Matrix<double>  result_inv(3, 3);
    result_inv = mat3.inv();

    cout << "The inverse matrix of mat3 is : " << endl;
    for (int i = 0; i < result_inv.rows(); i++)
    {
        for (int j = 0; j < result_inv.cols(); j++)
        {
            cout << result_inv(i, j) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

