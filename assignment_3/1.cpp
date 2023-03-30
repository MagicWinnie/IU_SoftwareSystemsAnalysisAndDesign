#include <iostream>
#include <iomanip>
#include <cmath>

const double EPS = 1E-10;

using namespace std;

class Matrix
{
protected:
    size_t rows = 0, cols = 0;
    double **matrix;

public:
    Matrix(size_t rows, size_t cols)
    {
        this->rows = rows;
        this->cols = cols;

        matrix = new double *[rows];
        for (size_t i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                matrix[i][j] = 0.0;
    }
    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;
    }
    size_t get_rows() const { return rows; }
    size_t get_cols() const { return cols; }
    friend istream &operator>>(istream &in, Matrix &m)
    {
        for (size_t i = 0; i < m.rows; i++)
            for (size_t j = 0; j < m.cols; j++)
                in >> m[i][j];
        return in;
    }
    friend ostream &operator<<(ostream &out, Matrix &m)
    {
        for (size_t i = 0; i < m.rows; i++)
        {
            for (size_t j = 0; j < m.cols; j++)
            {
                out << ((abs(m[i][j]) < EPS and signbit(m[i][j])) ? 0.0 : m[i][j]);
                if (j != m.cols - 1)
                    cout << ' ';
            }
            out << '\n';
        }
        return out;
    }
    double *operator[](size_t i) { return matrix[i]; }
    Matrix &operator=(Matrix &old)
    {
        if (this == &old)
            return *this;

        for (size_t i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;

        rows = old.rows;
        cols = old.cols;

        matrix = new double *[rows];
        for (size_t i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                matrix[i][j] = old[i][j];

        return *this;
    }
    Matrix *operator+(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                C->matrix[i][j] = matrix[i][j] + B[i][j];
        return C;
    }
    Matrix *operator-(Matrix &B)
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                C->matrix[i][j] = matrix[i][j] - B[i][j];
        return C;
    }
    Matrix *operator*(Matrix &B)
    {
        if (cols != B.rows)
            throw runtime_error("Error: the dimensional problem occurred");
        Matrix *C = new Matrix(rows, B.cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < B.cols; j++)
                for (size_t k = 0; k < cols; k++)
                    C->matrix[i][j] += matrix[i][k] * B[k][j];
        return C;
    }
    Matrix *T()
    {
        Matrix *B = new Matrix(cols, rows);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                B->matrix[j][i] = matrix[i][j];
        return B;
    }
};

class SquareMatrix : public Matrix
{
protected:
    size_t size;
    friend SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info);
    friend double determinant_worker(SquareMatrix *A, bool debug_info);

public:
    SquareMatrix(size_t size) : Matrix(size, size) { this->size = size; }
    size_t get_size() const { return size; }
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
    double determinant(bool debug_info) { return determinant_worker(this, debug_info); }
    SquareMatrix *inverse(bool debug_info) { return inverse_worker(this, debug_info); }
};

class IdentityMatrix : public SquareMatrix
{
public:
    IdentityMatrix(size_t size) : SquareMatrix(size)
    {
        for (size_t i = 0; i < size; i++)
            matrix[i][i] = 1.0;
    }
};

class EliminationMatrix : public IdentityMatrix
{
public:
    EliminationMatrix(size_t size, size_t i, size_t j, double val) : IdentityMatrix(size)
    {
        matrix[i][j] = val * -1.0;
    }
};

class PermutationMatrix : public IdentityMatrix
{
public:
    PermutationMatrix(size_t size, size_t i, size_t j) : IdentityMatrix(size)
    {
        swap(matrix[i], matrix[j]);
    }
};

class ColumnVector : public Matrix
{
public:
    ColumnVector(size_t size) : Matrix(size, 1) {}
    ColumnVector *operator+(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) + (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    ColumnVector *operator-(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) - (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    ColumnVector *operator*(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) * (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    ColumnVector *T()
    {
        Matrix *upcast_A = this;
        Matrix *upcast_C = (*upcast_A).T();
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    double norm()
    {
        double result = 0.0;
        for (size_t i = 0; i < rows; i++)
            result += i * i;
        return sqrt(result);
    }
};

template <typename L, typename R>
class AugmentedMatrix
{
protected:
    L *matrixLeft;
    R *matrixRight;

public:
    AugmentedMatrix(L *A, R *B)
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
    L *getLeft() const { return matrixLeft; }
    R *getRight() const { return matrixRight; }
    friend ostream &operator<<(ostream &out, AugmentedMatrix &m)
    {
        out << *m.getLeft() << *m.getRight();
        return out;
    }
    AugmentedMatrix<L, R> *ForwardElimination(bool debug_info, int *last_step)
    {
        AugmentedMatrix<L, R> *U = this;
        size_t curr_col = 0;
        size_t rows = matrixLeft->get_rows();
        for (size_t i = 0; i < rows; i++)
        {
            int row_with_max_pivot = -1;
            double max_pivot = (*U->matrixLeft)[i][curr_col];
            for (size_t j = i + 1; j < rows; j++)
            {
                if (abs((*U->matrixLeft)[j][curr_col]) < EPS)
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
                U->matrixLeft = (L *)((*(Matrix *)P) * (*(Matrix *)U->matrixLeft));
                U->matrixRight = (R *)((*(Matrix *)P) * (*(Matrix *)U->matrixRight));
                if (debug_info)
                {
                    cout << *U;
                    (*last_step)++;
                }
            }
            for (size_t j = i + 1; j < rows; j++)
            {
                if (abs((*U->matrixLeft)[j][curr_col]) < EPS or abs((*U->matrixLeft)[i][curr_col]) < EPS)
                    continue;
                if (debug_info)
                    cout << "step #" << *last_step << ": elimination\n";
                EliminationMatrix *E = new EliminationMatrix(rows, j, i, (*U->matrixLeft)[j][curr_col] / (*U->matrixLeft)[i][curr_col]);
                U->matrixLeft = (L *)((*(Matrix *)E) * (*(Matrix *)U->matrixLeft));
                U->matrixRight = (R *)((*(Matrix *)E) * (*(Matrix *)U->matrixRight));
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
    AugmentedMatrix<L, R> *BackwardElimination(bool debug_info, int *last_step)
    {
        AugmentedMatrix<L, R> *U = this;
        size_t curr_col = matrixLeft->get_cols() - 1;
        size_t rows = matrixLeft->get_rows();
        for (size_t i = rows; i >= 1; i--)
        {
            for (size_t j = i; j >= 1; j--)
            {
                if (abs((*U->matrixLeft)[j - 1][curr_col]) < EPS or abs((*U->matrixLeft)[i - 1][curr_col]) < EPS)
                    continue;
                if (debug_info)
                    cout << "step #" << *last_step << ": elimination\n";
                EliminationMatrix *E = new EliminationMatrix(rows, j - 1, i - 1, (*U->matrixLeft)[j - 1][curr_col] / (*U->matrixLeft)[i - 1][curr_col]);
                U->matrixLeft = (L *)((*(Matrix *)E) * (*(Matrix *)U->matrixLeft));
                U->matrixRight = (R *)((*(Matrix *)E) * (*(Matrix *)U->matrixRight));
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
    friend ColumnVector *solve(AugmentedMatrix<SquareMatrix, ColumnVector> *A, bool debug_info);
};

double determinant_worker(SquareMatrix *A, bool debug_info)
{
    int step = 0;
    AugmentedMatrix<SquareMatrix, SquareMatrix> *aug = new AugmentedMatrix(A, A);
    SquareMatrix *U = aug->ForwardElimination(debug_info, &step)->getLeft();
    double res = 1.0;
    for (size_t i = 0; i < A->get_size(); i++)
        res *= U->matrix[i][i];
    return res;
}

SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info)
{
    IdentityMatrix *I = new IdentityMatrix(A->get_size());
    SquareMatrix *I_matrix = I;
    if (debug_info)
        cout << "step #0: Augmented Matrix\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *aug = new AugmentedMatrix(A, I_matrix);
    if (debug_info)
        cout << *aug;

    int last_step = 1;
    if (debug_info)
        cout << "Direct way:\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *B = aug->ForwardElimination(debug_info, &last_step);

    if (debug_info)
        cout << "Way back:\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *C = B->BackwardElimination(debug_info, &last_step);

    if (debug_info)
        cout << "Diagonal normalization:\n";
    for (size_t i = 0; i < C->getLeft()->get_rows(); i++)
    {
        double pivot = (*C->getLeft())[i][i];
        for (size_t j = 0; j < C->getLeft()->get_cols(); j++)
            (*C->getLeft())[i][j] /= pivot;
        for (size_t j = 0; j < C->getRight()->get_cols(); j++)
            (*C->getRight())[i][j] /= pivot;
    }
    cout << *C;

    return (SquareMatrix *)C->getRight();
}

ColumnVector *solve(AugmentedMatrix<SquareMatrix, ColumnVector> *A, bool debug_info)
{
    if (debug_info)
    {
        cout << "step #0:\n";
        cout << *A;
    }

    int last_step = 1;
    AugmentedMatrix<SquareMatrix, ColumnVector> *B = A->ForwardElimination(debug_info, &last_step);
    AugmentedMatrix<SquareMatrix, ColumnVector> *C = B->BackwardElimination(debug_info, &last_step);

    if (debug_info)
        cout << "Diagonal normalization:\n";
    for (size_t i = 0; i < C->getLeft()->get_rows(); i++)
    {
        double pivot = (*C->getLeft())[i][i];
        if (abs(pivot) < EPS)
            continue;
        for (size_t j = 0; j < C->getLeft()->get_cols(); j++)
            (*C->getLeft())[i][j] /= pivot;
        (*C->getRight())[i][0] /= pivot;
    }
    bool exist_zeros = false;
    for (size_t i = 0; i < C->getLeft()->get_rows(); i++)
    {
        bool all_zeros = true;
        for (size_t j = 0; j < C->getLeft()->get_cols(); j++)
            if (abs((*C->getLeft())[i][j]) >= EPS)
            {
                all_zeros = false;
                break;
            }
        if (all_zeros and abs((*C->getRight())[i][0]) >= EPS)
            throw runtime_error("NO");
        if (all_zeros)
            exist_zeros = true;
    }
    if (exist_zeros)
        throw runtime_error("INF");
    if (debug_info)
        cout << *C;
    return A->matrixRight;
}

AugmentedMatrix<SquareMatrix, ColumnVector> *read_matrix()
{
    size_t size;
    cin >> size;

    SquareMatrix *matrix = new SquareMatrix(size);
    cin >> *matrix;

    ColumnVector *column = new ColumnVector(size);
    cin >> *column;

    AugmentedMatrix<SquareMatrix, ColumnVector> *aug = new AugmentedMatrix(matrix, column);

    return aug;
}

int main(void)
{
    cout << fixed << setprecision(2);

    AugmentedMatrix<SquareMatrix, ColumnVector> *A = read_matrix();

    try
    {
        ColumnVector *solution = solve(A, false);
        cout << *solution;
    }
    catch (const runtime_error &e)
    {
        cout << e.what();
    }

    return 0;
}