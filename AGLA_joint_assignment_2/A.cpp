#include <iostream>
#include <iomanip>
#include <cmath>
#define USE_GNUPLOT 1
#define GNUPLOT_NAME "gnuplot -persist"

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
                out << ((abs(m[i][j]) <= 1e-6 and signbit(m[i][j])) ? 0.0 : m[i][j]) << ' ';
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
    friend SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info);
    friend double determinant_worker(SquareMatrix *A, bool debug_info);

public:
    SquareMatrix(int size) : Matrix(size, size) {}
    int get_size()
    {
        return rows;
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

class ColumnVector : public Matrix
{
public:
    ColumnVector(int size) : Matrix(size, 1) {}
    double *operator[](int i)
    {
        return &matrix[i][0];
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
    L *getLeft()
    {
        return matrixLeft;
    }
    R *getRight()
    {
        return matrixRight;
    }
    friend ostream &operator<<(ostream &out, AugmentedMatrix &m)
    {
        out << *m.getLeft() << *m.getRight();
        return out;
    }
    AugmentedMatrix<L, R> *ForwardElimination(bool debug_info, int *last_step)
    {
        AugmentedMatrix<L, R> *U = this;
        int curr_col = 0;
        int rows = matrixLeft->get_rows();
        for (int i = 0; i < rows; i++)
        {
            int row_with_max_pivot = -1;
            double max_pivot = (*U->matrixLeft)[i][curr_col];
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
                U->matrixLeft = (L *)((*(Matrix *)P) * (*(Matrix *)U->matrixLeft));
                U->matrixRight = (R *)((*(Matrix *)P) * (*(Matrix *)U->matrixRight));
                if (debug_info)
                {
                    cout << *U;
                    (*last_step)++;
                }
            }
            for (int j = i + 1; j < rows; j++)
            {
                if ((*U->matrixLeft)[j][curr_col] == 0.0 || (*U->matrixLeft)[i][curr_col] == 0.0)
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
        int curr_col = matrixLeft->get_cols() - 1;
        int rows = matrixLeft->get_rows();
        for (int i = rows - 1; i >= 0; i--)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if ((*U->matrixLeft)[j][curr_col] == 0.0 || (*U->matrixLeft)[i][curr_col] == 0.0)
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
    for (int i = 0; i < A->get_size(); i++)
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
    for (int i = 0; i < C->getLeft()->get_rows(); i++)
    {
        double pivot = (*C->getLeft())[i][i];
        for (int j = 0; j < C->getLeft()->get_cols(); j++)
            (*C->getLeft())[i][j] /= pivot;
        for (int j = 0; j < C->getRight()->get_cols(); j++)
            (*C->getRight())[i][j] /= pivot;
    }
    if (debug_info)
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
    for (int i = 0; i < C->getLeft()->get_rows(); i++)
    {
        double pivot = (*C->getLeft())[i][i];
        if (pivot == 0.0)
            continue;
        for (int j = 0; j < C->getLeft()->get_cols(); j++)
            (*C->getLeft())[i][j] /= pivot;
        for (int j = 0; j < C->getRight()->get_cols(); j++)
            (*C->getRight())[i][j] /= pivot;
    }
    if (debug_info)
        cout << *C;

    return A->matrixRight;
}

int main(void)
{
    cout << fixed << setprecision(4);

    int m;
    cin >> m;
    double t[m];
    ColumnVector *b = new ColumnVector(m);
    for (int i = 0; i < m; i++)
    {
        double t_i, b_i;
        cin >> t_i >> b_i;
        t[i] = t_i;
        (*b->operator[](i)) = b_i;
    }
    int n;
    cin >> n;

    Matrix *A = new Matrix(m, n + 1);
    for (int i = 0; i <= n; i++)
        for (int j = 0; j < m; j++)
            A->operator[](j)[i] = pow(t[j], (double)i);

    cout << "A:\n";
    cout << *A;

    Matrix *A_T = A->T();
    SquareMatrix *A_1 = (SquareMatrix *)((*A_T) * (*A));

    cout << "A_T*A:\n";
    cout << *A_1;

    SquareMatrix *A_2 = A_1->inverse(false);

    cout << "(A_T*A)^-1:\n";
    cout << *A_2;

    ColumnVector *A_3 = (ColumnVector *)((*(Matrix *)A_T) * (*(Matrix *)b));

    cout << "A_T*b:\n";
    cout << *A_3;

    ColumnVector *A_4 = (ColumnVector *)((*(Matrix *)A_2) * (*(Matrix *)A_3));

    cout << "x~:\n";
    cout << *A_4;

#if (defined(WIN32) || defined(_WIN32)) && USE_GNUPLOT
    FILE *pipe = _popen(GNUPLOT_NAME, "w");
#elif USE_GNUPLOT
    FILE *pipe = popen(GNUPLOT_NAME, "w");
#endif
#if USE_GNUPLOT
    fprintf(pipe, "%s\n", "set terminal png");
    fprintf(pipe, "%s\n", "set output 'output.png'");
    fprintf(pipe, "%s\n", "set title \"Least Squares Approximation\"");
    fprintf(pipe, "%s\n", "set key noautotitle");
    fprintf(pipe, "%s\n", "set autoscale xy");
    fprintf(pipe, "%s\n", "set offsets 0.05, 0.05, 0.05, 0.05");
    string func;
    for (int i = 0; i <= n; i++)
    {
        if (*A_4->operator[](i) < 0 and i != 0)
            func = func.substr(0, func.size() - 1);
        func += to_string(*A_4->operator[](i));
        func += '*';
        func += "x**";
        func += to_string(i);
        if (i != n)
            func += '+';
    }
    cout << func << endl;
    fprintf(pipe, "plot %s lw 3, '-' w p pt 7 ps 2\n", func.c_str());
    for (int i = 0; i < m; i++)
        fprintf(pipe, "%lf %lf\n", t[i], *b->operator[](i));
    fprintf(pipe, "%s\n", "e");
    fflush(pipe);
#endif
#if (defined(WIN32) || defined(_WIN32)) && USE_GNUPLOT
    _pclose(pipe);
#elif USE_GNUPLOT
    pclose(pipe);
#endif

    return 0;
}