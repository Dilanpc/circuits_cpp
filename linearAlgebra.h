
#ifndef LINEARALGEBRA_H
#define LINEARALGEBRA_H

#include <vector>
#include <cmath>
#include <iostream>


using std::vector;

double round(double value, int decimalPlaces);



class Matrix : public vector<vector<double>>
{
public:
    Matrix() : vector<vector<double>>(){};
    Matrix(int rows, int cols);
    Matrix(vector<double> vector);
    Matrix(vector<vector<double>> matrix);
    Matrix(const std::initializer_list<std::initializer_list<double>>& list)
        : vector<vector<double>>(list.begin(), list.end()) {}

    Matrix operator+(const Matrix& matrix) const;
    Matrix operator*(const Matrix& matrix) const;
    Matrix operator/(const Matrix& matrix) const;
    Matrix operator-(const Matrix& matrix) const;
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    Matrix transpose() const;
    Matrix minor(int row, int col) const;
    double determinant() const;
    double det() const { return determinant(); }
    Matrix inverse() const;
    Matrix inv() const { return inverse(); }
    Matrix solve(const Matrix& matrix) const { return inverse() * matrix; }

    void identity();
    void stack(const Matrix& matrix, bool vertical);



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

};




inline Matrix::Matrix(int rows, int cols) : vector<vector<double>>(rows, vector<double>(cols)){}

inline Matrix::Matrix(vector<double> _vector) : vector<vector<double>>(1, _vector){}

inline Matrix::Matrix(vector<vector<double>> matrix) : vector<vector<double>>(matrix){}


Matrix operator*(double scalar, const Matrix& matrix);

#endif
