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
    SquareMatrix *B = read_matrix();
    SquareMatrix *C = read_matrix();

    try
    {
        SquareMatrix *D = (*A) + (*B);
        cout << *D;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    try
    {
        SquareMatrix *E = (*B) - (*A);
        cout << *E;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    try
    {
        SquareMatrix *F = (*C) * (*A);
        cout << *F;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    SquareMatrix *G = A->T();
    cout << *G;

    return 0;
}