#include "linearAlgebra.h"



double round(double value, int decimalPlaces) {
    double multiplier = std::pow(10.0, decimalPlaces);
    return std::round(value * multiplier) / multiplier;
}

// OPERATORS

Matrix Matrix::operator+(const Matrix& matrix) const
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] + matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& matrix) const
{
    if (size() != matrix.size() || at(0).size() != matrix.at(0).size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] - matrix[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    if (at(0).size() != matrix.size())
    {
        throw "Matrix dimensions must agree";
    }
    Matrix result(size(), matrix.at(0).size());
    for (int i = 0; i < size(); i++) // rows iter
    {

        for (int j = 0; j < matrix.at(0).size(); j++)  // cols iter
        {
            for (int k = 0; k < matrix.size(); k++)
            {
                result[i][j] += at(i)[k] * matrix[k][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator/(const Matrix& matrix) const
{
    return (*this) * matrix.inverse();
}

Matrix Matrix::operator*(double scalar) const
{
    Matrix result(size(), at(0).size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = at(i)[j] * scalar;
        }
    }
    return result;
}

Matrix operator*(double scalar, const Matrix& matrix) { return matrix * scalar; }

Matrix Matrix::operator/(double scalar) const
{
    return (*this) * (1 / scalar);
}


// METHODS

Matrix Matrix::transpose() const
{
    Matrix result(at(0).size(), size());
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[j][i] = at(i)[j];
        }
    }
    return result;
}

Matrix Matrix::minor(int row, int col) const
{
    Matrix result(size() - 1, at(0).size() - 1);
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            if (i != row && j != col)
            {
                result[i < row ? i : i - 1][j < col ? j : j - 1] = at(i)[j];
            }
        }
    }
    return result;
}

double Matrix::determinant(const Matrix& matrix) const
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
    for (int i = 0; i < matrix.size(); i++)
    {
        det += (i % 2 ? -1 : 1) * matrix[0][i] * determinant(matrix.minor(0, i));
    }
    return det;
}

double Matrix::determinant() const
{
    return determinant(*this);
}


Matrix Matrix::inverse() const
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
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            result[i][j] = ((i + j) % 2 ? -1 : 1) * determinant(minor(i, j)) / det;
        }
    }
    return result.transpose();
}


void Matrix::identity()
{
    for (int i = 0; i < size(); i++)
    {
        for (int j = 0; j < at(0).size(); j++)
        {
            at(i)[j] = (i == j ? 1 : 0);
        }
    }
}


void Matrix::stack(const Matrix& matrix, bool vertical)
{
    if (vertical)
    {
        if (at(0).size() != matrix.at(0).size()) {
            throw "Matrix dimensions must agree";
        }
        insert(end(), matrix.begin(), matrix.end());
    }
    else
    {
        if (size() != matrix.size()) {
            throw "Matrix dimensions must agree";
        }
        for (int i = 0; i < size(); i++)
        {
            at(i).insert(at(i).end(), matrix[i].begin(), matrix[i].end());
        }
    }
}
