#include <iostream>
#include "matrix.h"

using namespace std;

int main()
{    
    Matrix<int, 0> matrix;
    static constexpr size_t matrixSize = 10; 
    for (size_t i = 0, j = matrixSize-1; i < matrixSize; ++i, --j)
    {
        matrix[i][i] = i;
        matrix[i][j] = j;
    }

    for (size_t i = 1; i < matrixSize-1; ++i)
    {
        for (size_t j = 1; j < matrixSize-1; ++j)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    cout << matrix.size() << endl;

    for(const auto& cell : matrix)
    {
        size_t i,j;
        int v;
        tie(i, j, v) = (tuple<size_t, size_t, int>)cell;
        cout << i << " " << j << " " << v << endl;
    }

    // ((matrix[100][100] = 314) = 0) = 217;
    // cout << "at matrix[100][100] = " << matrix[100][100] << endl;

    return 0;
}
