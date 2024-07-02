
#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <vector>
#include <cmath>
#include <iostream>



double round(double value, int decimalPlaces);



class Matrix : public std::vector<std::vector<double>>
{
public:
    Matrix() : std::vector<std::vector<double>>(){};
    Matrix(int rows, int cols);
    Matrix(const std::vector<double>& _vector);
    Matrix(const std::vector<std::vector<double>>& matrix);
    Matrix(const std::initializer_list<std::initializer_list<double>>& list)
        : std::vector<std::vector<double>>(list.begin(), list.end()) {}

    Matrix operator+(const Matrix& matrix) const;
    Matrix operator*(const Matrix& matrix) const;
    Matrix operator/(const Matrix& matrix) const;
    Matrix operator-(const Matrix& matrix) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    Matrix transpose() const;
    Matrix minor(int row, int col) const;
    double determinant() const;
    double det() const;
    Matrix inverse() const;
    Matrix inverseAdjugate() const;
    Matrix inv() const;
    Matrix solve(const Matrix& _vector) const;

    void identity();
    static Matrix identity(int size);
    void stack(const Matrix& matrix, bool vertical);

    std::vector<double> getRow(const int row) const;
    std::vector<double> getColumn(const int col) const;




    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
    {
        for (int i=0; i<matrix.size(); i++)
        {
            for (int j=0; j<matrix.at(0).size(); j++)
            {
                os << round(matrix[i][j], 3) << " ";
            }
            os << std::endl;
        }
        return os;
    }



protected:
    double determinant(const Matrix& matrix) const;

    void gauss();


    void multiplyRow(int row, double scalar);
    void multiplyRow(std::vector<double>& _vector, double scalar) const;
    void sumRow(int row1, int row2);
    void sumRow(int row, const std::vector<double>& _vector);


};




inline Matrix::Matrix(int rows, int cols) : std::vector<std::vector<double>>(rows, std::vector<double>(cols)){}

inline Matrix::Matrix(const std::vector<double>& _vector) : std::vector<std::vector<double>>(1, _vector){}

inline Matrix::Matrix(const std::vector<std::vector<double>>& matrix) : std::vector<std::vector<double>>(matrix){}


Matrix operator*(double scalar, const Matrix& matrix);
Matrix operator/(double scalar, const Matrix& matrix);

#endif
