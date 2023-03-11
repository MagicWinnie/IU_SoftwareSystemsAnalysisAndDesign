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
                C->matrix[i][j] = matrix[i][j] + B[i][j];
        return C;
    }
    Matrix *operator-(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, cols);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                C->matrix[i][j] = matrix[i][j] - B[i][j];
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
                    C->matrix[i][j] += matrix[i][k] * B[k][j];
        return C;
    }
    Matrix *T()
    {
        Matrix *B = new Matrix(cols, rows);
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                B->matrix[j][i] = matrix[i][j];
        return B;
    }
};

class SquareMatrix : public Matrix
{
protected:
    int size;
    friend SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info);
    friend double determinant_worker(SquareMatrix *A, bool debug_info);

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
        return determinant_worker(this, debug_info);
    }
    SquareMatrix *inverse(bool debug_info)
    {
        return inverse_worker(this, debug_info);
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

class AugmentedMatrix
{
protected:
    Matrix *matrixLeft;
    Matrix *matrixRight;

public:
    AugmentedMatrix(Matrix *A, Matrix *B)
    {
        if (A->get_rows() != B->get_rows())
            throw runtime_error("Error: the dimensional problem occurred");
        matrixLeft = A;
        matrixRight = B;
    }
    ~AugmentedMatrix()
    {
        delete[] matrixLeft;
        delete[] matrixRight;
    }
    Matrix *getLeft()
    {
        return matrixLeft;
    }
    Matrix *getRight()
    {
        return matrixRight;
    }
    friend ostream &operator<<(ostream &out, AugmentedMatrix &m)
    {
        for (int i = 0; i < m.matrixLeft->get_rows(); i++)
        {
            for (int j = 0; j < m.matrixLeft->get_cols(); j++)
                out << (*m.matrixLeft)[i][j] << ' ';
            for (int j = 0; j < m.matrixRight->get_cols(); j++)
                out << (*m.matrixRight)[i][j] << ' ';
            out << '\n';
        }
        return out;
    }
    friend AugmentedMatrix *ForwardElimination(AugmentedMatrix *A, bool debug_info, int *last_step);
    friend AugmentedMatrix *BackwardElimination(AugmentedMatrix *A, bool debug_info, int *last_step);
};

double determinant_worker(SquareMatrix *A, bool debug_info)
{
    int step = 0;
    AugmentedMatrix *aug = new AugmentedMatrix(A, A);
    SquareMatrix *U = (SquareMatrix *)ForwardElimination(aug, true, &step);
    double res = 1.0;
    for (int i = 0; i < A->get_size(); i++)
        res *= U->matrix[i][i];
    return res;
}

AugmentedMatrix *ForwardElimination(AugmentedMatrix *A, bool debug_info, int *last_step)
{
    AugmentedMatrix *U = A;
    int curr_col = 0;
    int rows = A->getLeft()->get_rows();
    for (int i = 0; i < rows; i++)
    {
        int row_with_max_pivot = -1;
        int max_pivot = (*U->matrixLeft)[i][curr_col];
        for (int j = i + 1; j < rows; j++)
        {
            if ((*U->matrixLeft)[j][curr_col] == 0.0)
                continue;
            if (abs((*U->matrixLeft)[j][curr_col]) > abs(max_pivot))
            {
                row_with_max_pivot = j;
                max_pivot = (*U->matrixLeft)[j][curr_col];
            }
        }
        if (row_with_max_pivot != -1)
        {
            if (debug_info)
                cout << "step #" << *last_step << ": permutation\n";
            PermutationMatrix *P = new PermutationMatrix(rows, i, row_with_max_pivot);
            U->matrixLeft = (*(Matrix *)P) * (*U->matrixLeft);
            U->matrixRight = (*(Matrix *)P) * (*U->matrixRight);
            if (debug_info)
            {
                cout << *U;
                (*last_step)++;
            }
        }
        for (int j = i + 1; j < rows; j++)
        {
            if ((*U->matrixLeft)[j][curr_col] == 0.0)
                continue;
            if (debug_info)
                cout << "step #" << *last_step << ": elimination\n";
            EliminationMatrix *E = new EliminationMatrix(rows, j, i, (*U->matrixLeft)[j][curr_col] / (*U->matrixLeft)[i][curr_col]);
            U->matrixLeft = (*(Matrix *)E) * (*U->matrixLeft);
            U->matrixRight = (*(Matrix *)E) * (*U->matrixRight);
            if (debug_info)
            {
                cout << *U;
                (*last_step)++;
            }
        }
        curr_col++;
    }
    return U;
}

AugmentedMatrix *BackwardElimination(AugmentedMatrix *A, bool debug_info, int *last_step)
{
    AugmentedMatrix *U = A;
    int curr_col = A->matrixLeft->get_cols() - 1;
    int rows = A->matrixLeft->get_rows();
    for (int i = rows - 1; i >= 0; i--)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            if ((*U->matrixLeft)[j][curr_col] == 0.0 || (*U->matrixLeft)[i][curr_col] == 0.0)
                continue;
            if (debug_info)
                cout << "step #" << *last_step << ": elimination\n";
            EliminationMatrix *E = new EliminationMatrix(rows, j, i, (*U->matrixLeft)[j][curr_col] / (*U->matrixLeft)[i][curr_col]);
            U->matrixLeft = (*(Matrix *)E) * (*U->matrixLeft);
            U->matrixRight = (*(Matrix *)E) * (*U->matrixRight);
            if (debug_info)
            {
                cout << *U;
                (*last_step)++;
            }
        }
        curr_col--;
    }
    return U;
}

SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info)
{
    IdentityMatrix *I = new IdentityMatrix(A->get_size());
    if (debug_info)
        cout << "step #0: Augmented Matrix\n";
    AugmentedMatrix *aug = new AugmentedMatrix(A, I);
    if (debug_info)
        cout << *aug;

    int last_step = 1;
    if (debug_info)
        cout << "Direct way:\n";
    AugmentedMatrix *B = ForwardElimination(aug, debug_info, &last_step);

    if (debug_info)
        cout << "Way back:\n";
    AugmentedMatrix *C = BackwardElimination(B, debug_info, &last_step);

    if (debug_info)
        cout << "Diagonal normalization:\n";
    for (int i = 0; i < C->getLeft()->get_rows(); i++)
    {
        double pivot = (*C->getLeft())[i][i];
        for (int j = 0; j < C->getLeft()->get_cols(); j++)
            (*C->getLeft())[i][j] /= pivot;
        for (int j = 0; j < C->getRight()->get_cols(); j++)
            (*C->getRight())[i][j] /= pivot;
    }
    cout << *C;

    return (SquareMatrix *)C->getRight();
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

    SquareMatrix *inverse = A->inverse(true);
    cout << "result:\n"
         << *inverse;

    return 0;
}