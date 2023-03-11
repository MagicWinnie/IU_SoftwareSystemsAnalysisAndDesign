#include <iostream>

using namespace std;

class Matrix
{
protected:
    int rows = 0, cols = 0;
    int **matrix;

public:
    Matrix(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;

        matrix = new int *[rows];
        for (int i = 0; i < rows; i++)
            matrix[i] = new int[cols];

        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i][j] = 0;
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
    int *operator[](int i)
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

        matrix = new int *[rows];
        for (int i = 0; i < rows; i++)
            matrix[i] = new int[cols];

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
};

class SquareMatrix : public Matrix
{
public:
    SquareMatrix(int size) : Matrix(size, size)
    {
    }
    int get_size()
    {
        return cols;
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
};

class IdentityMatrix : public SquareMatrix
{
public:
    IdentityMatrix(int size) : SquareMatrix(size)
    {
        for (int i = 0; i < size; i++)
            matrix[i][i] = 1;
    }
};

class EliminationMatrix : public IdentityMatrix
{
public:
    EliminationMatrix(int size, int i, int j, SquareMatrix *M) : IdentityMatrix(size)
    {
        matrix[i - 1][j - 1] = (*M)[i - 1][j - 1] * -1;
    }
};

class PermutationMatrix : public IdentityMatrix
{
public:
    PermutationMatrix(int size, int i, int j) : IdentityMatrix(size)
    {
        swap(matrix[i - 1], matrix[j - 1]);
    }
};

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
    SquareMatrix *A = read_matrix();

    IdentityMatrix *I = new IdentityMatrix(3);
    cout << *I;

    EliminationMatrix *E_21 = new EliminationMatrix(A->get_size(), 2, 1, A);
    cout << *E_21;

    SquareMatrix *B = (*E_21) * (*A);
    cout << *B;

    PermutationMatrix *P_21 = new PermutationMatrix(A->get_size(), 2, 1);
    cout << *P_21;

    SquareMatrix *C = (*P_21) * (*A);
    cout << *C;

    return 0;
}