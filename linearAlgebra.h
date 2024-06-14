#include <vector>
#include <cmath>
#include <iostream>


using std::vector;

double round(double value, int decimalPlaces) {
    double multiplier = std::pow(10.0, decimalPlaces);
    return std::round(value * multiplier) / multiplier;
}


class Matrix : public vector<vector<double>>
{
public:
    Matrix(int rows, int cols);
    Matrix(vector<double> vector);
    Matrix(vector<vector<double>> matrix);
    Matrix(const std::initializer_list<std::initializer_list<double>>& list)
        : vector<vector<double>>(list.begin(), list.end()) {}

    Matrix operator+(Matrix matrix);
    Matrix operator-(Matrix matrix);
    Matrix operator*(Matrix matrix);
    Matrix operator/(Matrix matrix);
    Matrix operator*(double scalar);
    Matrix operator/(double scalar);

    Matrix transpose();
    Matrix minor(int row, int col);
    double determinant();
    double det(){ return determinant(); }
    Matrix inverse();
    Matrix inv(){ return inverse(); }
    Matrix solve(Matrix matrix){ return inverse() * matrix; }

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
    double determinant(Matrix matrix);

};




Matrix::Matrix(int rows, int cols) : vector<vector<double>>(rows, vector<double>(cols)){}

Matrix::Matrix(vector<double> _vector) : vector<vector<double>>(1, _vector){}

Matrix::Matrix(vector<vector<double>> matrix) : vector<vector<double>>(matrix){}


// OPERATORS

Matrix Matrix::operator+(Matrix matrix)
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            result[i][j] = at(i)[j] + matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(Matrix matrix)
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            result[i][j] = at(i)[j] - matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(Matrix matrix)
{
    if (at(0).size() != matrix.size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), matrix.at(0).size());
    for (int i=0; i<size(); i++) // rows iter
    {

        for (int j=0; j<at(0).size(); j++)  // cols iter
        {
            for (int k=0; k<matrix.size(); k++)
            {
                result[i][j] += at(i)[k] * matrix[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator/(Matrix matrix)
{
    return (*this) * matrix.inverse();
}

Matrix Matrix::operator*(double scalar)
{
    Matrix result(size(), at(0).size());
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            result[i][j] = at(i)[j] * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, Matrix matrix){ return matrix * scalar; }

Matrix Matrix::operator/(double scalar)
{
    return (*this) * (1/scalar);
}


// METHODS

Matrix Matrix::transpose()
{
    Matrix result(at(0).size(), size());
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            result[j][i] = at(i)[j];
        }
    }
    return result;
}

Matrix Matrix::minor(int row, int col)
{
    Matrix result(size()-1, at(0).size()-1);
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            if (i != row && j != col)
            {
                result[i < row ? i : i-1][j < col ? j : j-1] = at(i)[j];
            }
        }
    }
    return result;
}

double Matrix::determinant(Matrix matrix)
{
    if (matrix.size() != matrix.at(0).size())
    {
        throw "Matrix must be square";
    }
    // Base case
    if (matrix.size() == 1)
    {
        return matrix[0][0];
    }
    if (matrix.size() == 2)
    {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    // Method: Cofactors
    double det = 0;
    for (int i=0; i<matrix.size(); i++)
    {
        det += (i%2 ? -1 : 1) * matrix[0][i] * determinant(minor(0, i));
    }
    return det;
}

double Matrix::determinant()
{
    return determinant(*this);
}


Matrix Matrix::inverse()
{
    if (size() != at(0).size())
    {
        throw "Matrix must be square";
    }
    double det = determinant();
    if (det == 0)
    {
        throw "Matrix is singular";
    }
    Matrix result(size(), at(0).size());
    // method: Adjugate matrix
    for (int i=0; i<size(); i++)
    {
        for (int j=0; j<at(0).size(); j++)
        {
            result[i][j] = ((i+j)%2 ? -1 : 1) * determinant(minor(i, j)) / det;
        }
    }
    return result.transpose();
}


