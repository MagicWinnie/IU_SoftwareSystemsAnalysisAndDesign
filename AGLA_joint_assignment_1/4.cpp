#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class Matrix
{
protected:
    int rows = 0, cols = 0;
    double **matrix;

public:
    Matrix(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;

        matrix = new double *[rows];
        for (int i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i][j] = 0.0;
    }
    ~Matrix()
    {
        for (int i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;
    }
    int get_rows() const
    {
        return rows;
    }
    int get_cols() const
    {
        return cols;
    }
    friend istream &operator>>(istream &in, Matrix &m)
    {
        for (int i = 0; i < m.rows; i++)
            for (int j = 0; j < m.cols; j++)
                in >> m[i][j];
        return in;
    }
    friend ostream &operator<<(ostream &out, Matrix &m)
    {
        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
                out << m[i][j] << ' ';
            out << '\n';
        }
        return out;
    }
    double *operator[](int i)
    {
        return matrix[i];
    }
    Matrix &operator=(Matrix &old)
    {
        if (this == &old)
            return *this;

        for (int i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;

        rows = old.rows;
        cols = old.cols;

        matrix = new double *[rows];
        for (int i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i][j] = old[i][j];

        return *this;
    }
    Matrix *operator+(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                (*C)[i][j] = matrix[i][j] + B[i][j];
        return C;
    }
    Matrix *operator-(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                (*C)[i][j] = matrix[i][j] - B[i][j];
        return C;
    }
    Matrix *operator*(Matrix &B)
    {
        if (cols != B.rows)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, B.cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < B.cols; j++)
                for (int k = 0; k < cols; k++)
                    (*C)[i][j] += matrix[i][k] * B[k][j];
        return C;
    }
    Matrix *T()
    {
        Matrix *B = new Matrix(cols, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                (*B)[j][i] = matrix[i][j];
        return B;
    }
    friend Matrix *DirectElimination(Matrix *A, bool debug_info);
};

class SquareMatrix : public Matrix
{
protected:
    int size;

public:
    SquareMatrix(int size) : Matrix(size, size)
    {
        this->size = size;
    }
    int get_size()
    {
        return size;
    }
    SquareMatrix *operator+(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) + (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    SquareMatrix *operator-(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) - (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    SquareMatrix *operator*(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) * (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    SquareMatrix *T()
    {
        Matrix *upcast_A = this;
        Matrix *upcast_C = (*upcast_A).T();
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    double determinant(bool debug_info)
    {
        SquareMatrix *U = (SquareMatrix *)DirectElimination(this, true);
        double res = 1.0;
        for (int i = 0; i < size; i++)
            res *= (*U)[i][i];
        return res;
    }
};

class IdentityMatrix : public SquareMatrix
{
public:
    IdentityMatrix(int size) : SquareMatrix(size)
    {
        for (int i = 0; i < size; i++)
            matrix[i][i] = 1.0;
    }
};

class EliminationMatrix : public IdentityMatrix
{
public:
    EliminationMatrix(int size, int i, int j, double val) : IdentityMatrix(size)
    {
        matrix[i][j] = val * -1.0;
    }
};

class PermutationMatrix : public IdentityMatrix
{
public:
    PermutationMatrix(int size, int i, int j) : IdentityMatrix(size)
    {
        swap(matrix[i], matrix[j]);
    }
};

Matrix *DirectElimination(Matrix *A, bool debug_info)
{
    Matrix *U = A;
    int curr_col = 0;
    int step = 1;
    int rows = A->get_rows();
    for (int i = 0; i < rows; i++)
    {
        int row_with_max_pivot = -1;
        int max_pivot = U->matrix[i][curr_col];
        for (int j = i + 1; j < rows; j++)
        {
            if (U->matrix[j][curr_col] == 0.0)
                continue;
            if (abs(U->matrix[j][curr_col]) > abs(max_pivot))
            {
                row_with_max_pivot = j;
                max_pivot = U->matrix[j][curr_col];
            }
        }
        if (row_with_max_pivot != -1)
        {
            if (debug_info)
                cout << "step #" << step << ": permutation\n";
            PermutationMatrix *P = new PermutationMatrix(rows, i, row_with_max_pivot);
            U = (*(Matrix *)P) * (*U);
            if (debug_info)
            {
                cout << *U;
                step++;
            }
        }
        for (int j = i + 1; j < rows; j++)
        {
            if (U->matrix[j][curr_col] == 0.0 || U->matrix[i][curr_col] == 0.0)
                continue;
            if (debug_info)
                cout << "step #" << step << ": elimination\n";
            EliminationMatrix *E = new EliminationMatrix(rows, j, i, (*U)[j][curr_col] / (*U)[i][curr_col]);
            U = (*(Matrix *)E) * (*U);
            if (debug_info)
            {
                cout << *U;
                step++;
            }
        }
        curr_col++;
    }
    return U;
}

SquareMatrix *read_matrix()
{
    int size;
    cin >> size;

    SquareMatrix *matrix = new SquareMatrix(size);
    cin >> *matrix;

    return matrix;
}

int main(void)
{
    cout << fixed << setprecision(2);

    SquareMatrix *A = read_matrix();

    double det = A->determinant(true);
    cout << "result:\n"
         << det << endl;

    return 0;
}