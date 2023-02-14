#include <iostream>

using namespace std;

class Matrix
{
private:
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
    Matrix operator+(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix C(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                C[i][j] = matrix[i][j] + B[i][j];
        return C;
    }
    Matrix operator-(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix C(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                C[i][j] = matrix[i][j] - B[i][j];
        return C;
    }
    Matrix operator*(Matrix &B)
    {
        if (cols != B.rows)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix C(rows, B.cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < B.cols; j++)
                for (int k = 0; k < cols; k++)
                    C[i][j] += matrix[i][k] * B[k][j];
        return C;
    }
    Matrix T()
    {
        Matrix B(cols, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                B[j][i] = matrix[i][j];
        return B;
    }
};

Matrix read_matrix()
{
    int rows, cols;
    cin >> rows >> cols;

    Matrix matrix(rows, cols);
    cin >> matrix;

    return matrix;
}

int main(void)
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    Matrix A = read_matrix();
    Matrix B = read_matrix();
    Matrix C = read_matrix();

    try
    {
        Matrix D = A + B;
        cout << D;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    try
    {
        Matrix E = B - A;
        cout << E;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    try
    {
        Matrix F = C * A;
        cout << F;
    }
    catch (exception &e)
    {
        cout << e.what() << endl;
    }
    Matrix G = A.T();
    cout << G;

    return 0;
}