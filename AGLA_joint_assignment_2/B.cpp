// Dmitriy Okoneshnikov
// d.okoneshnikov@innopolis.university
#include <iostream>
#include <iomanip>
#include <cmath>

// Epsilon for comparing double values
const double EPS = 1E-10;

using namespace std;

// The basic class for a general matrix containing double values.
// Not using templates as there is no need for it.
// In maths matrices are used with floating point values.
class Matrix
{
protected:
    // Size of matrix.
    size_t rows = 0, cols = 0;
    // 2D dynamic array for storing the matrix.
    double **matrix;

public:
    // Constructor.
    Matrix() {}
    Matrix(size_t rows, size_t cols)
    {
        this->rows = rows;
        this->cols = cols;

        matrix = new double *[rows];
        for (size_t i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        // Contains zeros as default values.
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                matrix[i][j] = 0.0;
    }
    // Getter for number of rows.
    size_t get_rows() const { return rows; }
    // Getter for number of columns.
    size_t get_cols() const { return cols; }
    // Cin operator overload.
    friend istream &operator>>(istream &in, Matrix &m)
    {
        for (size_t i = 0; i < m.rows; i++)
            for (size_t j = 0; j < m.cols; j++)
                in >> m[i][j];
        return in;
    }
    // Cout operator overload.
    friend ostream &operator<<(ostream &out, Matrix &m)
    {
        for (size_t i = 0; i < m.rows; i++)
        {
            for (size_t j = 0; j < m.cols; j++)
            {
                // Correct output for small negative values.
                out << ((abs(m[i][j]) < EPS and signbit(m[i][j])) ? 0.0 : m[i][j]);
                if (j != m.cols - 1)
                    cout << ' ';
            }
            out << '\n';
        }
        return out;
    }
    // Method for accessing the values by [].
    double *operator[](size_t i) { return matrix[i]; }
    const double *operator[](size_t i) const { return matrix[i]; }
    // = operator overloading.
    Matrix &operator=(const Matrix &old)
    {
        if (this == &old)
            return *this;

        if (rows == 0 || cols == 0)
        {
            rows = old.rows;
            cols = old.cols;

            matrix = new double *[rows];
            for (size_t i = 0; i < rows; i++)
                matrix[i] = new double[cols];
        }

        if (rows != old.rows or cols != old.cols)
            throw runtime_error("[ERROR] [operator=]: the dimensional problem occurred");

        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                matrix[i][j] = old.matrix[i][j];

        return *this;
    }
    // + operator overloading.
    Matrix operator+(const Matrix &B) const
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("[ERROR] [operator+]: the dimensional problem occurred");
        Matrix C(rows, cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
            {
                C.matrix[i][j] = matrix[i][j] + B[i][j];
                if (abs(C.matrix[i][j]) < EPS)
                    C.matrix[i][j] = 0;
            }
        return C;
    }
    // - operator overloading.
    Matrix operator-(const Matrix &B) const
    {
        if (rows != B.rows or cols != B.cols)
            throw runtime_error("[ERROR] [operator-]: the dimensional problem occurred");
        Matrix C(rows, cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
            {
                C.matrix[i][j] = matrix[i][j] - B[i][j];
                if (abs(C.matrix[i][j]) < EPS)
                    C.matrix[i][j] = 0;
            }
        return C;
    }
    // * operator overloading.
    Matrix operator*(const Matrix &B) const
    {
        if (cols != B.rows)
            throw runtime_error("[ERROR] [operator*]: the dimensional problem occurred");
        Matrix C(rows, B.cols);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < B.cols; j++)
                for (size_t k = 0; k < cols; k++)
                {
                    C.matrix[i][j] += matrix[i][k] * B[k][j];
                    if (abs(C.matrix[i][j]) < EPS)
                        C.matrix[i][j] = 0;
                }
        return C;
    }
    // Method to transpose the matrix.
    Matrix T() const
    {
        Matrix B(cols, rows);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                B.matrix[j][i] = matrix[i][j];
        return B;
    }
};

// Class for a square matrix that inherites the general matrix class.
class SquareMatrix : public Matrix
{
protected:
    // Method to get the inverse of the matrix.
    friend SquareMatrix inverse_worker(SquareMatrix A, bool debug_info);
    // Method to get the determinant of the matrix.
    friend double determinant_worker(SquareMatrix A, bool debug_info);

public:
    // Constructor.
    SquareMatrix() {}
    SquareMatrix(size_t size) : Matrix(size, size) {}
    SquareMatrix(Matrix matrix) : SquareMatrix(matrix.get_rows())
    {
        if (matrix.get_cols() != matrix.get_rows())
            throw runtime_error("[ERROR] [SquareMatrix(Matrix)]: the dimensional problem occurred");
        for (int i = 0; i < matrix.get_rows(); i++)
            for (int j = 0; j < matrix.get_cols(); j++)
                this->matrix[i][j] = matrix[i][j];
    }
    using Matrix::operator=;
    // + operator overloading using downcasting and upcasting.
    SquareMatrix operator+(const SquareMatrix &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) + (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const SquareMatrix *C_ptr = (SquareMatrix *)(upcast_C_ptr);

        SquareMatrix C(cols);
        C = *C_ptr;

        return C;
    }
    // - operator overloading using downcasting and upcasting.
    SquareMatrix operator-(const SquareMatrix &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) - (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const SquareMatrix *C_ptr = (SquareMatrix *)(upcast_C_ptr);

        SquareMatrix C(cols);
        C = *C_ptr;

        return C;
    }
    // * operator overloading using downcasting and upcasting.
    SquareMatrix operator*(const SquareMatrix &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) * (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const SquareMatrix *C_ptr = (SquareMatrix *)(upcast_C_ptr);

        SquareMatrix C(cols);
        C = *C_ptr;
        return C;
    }
    // Transpose method overloading using downcasting and upcasting.
    SquareMatrix T() const
    {
        const Matrix *upcast_A = this;

        const Matrix upcast_C = upcast_A->T();
        const Matrix *upcast_C_ptr = &upcast_C;

        const SquareMatrix *C_ptr = (SquareMatrix *)(upcast_C_ptr);

        SquareMatrix C(cols);
        C = *C_ptr;

        return C;
    }
    // Method to get the determinant of the matrix.
    double determinant(bool debug_info) { return determinant_worker(*this, debug_info); }
    // Method to get the inverse of the matrix.
    SquareMatrix inverse(bool debug_info) { return inverse_worker(*this, debug_info); }
};

// Class for an identity matrix that inherites the square matrix class.
class IdentityMatrix : public SquareMatrix
{
public:
    // Constructor.
    IdentityMatrix(size_t size) : SquareMatrix(size)
    {
        // Main diagonal has only 1s.
        for (size_t i = 0; i < size; i++)
            matrix[i][i] = 1.0;
    }
    IdentityMatrix(Matrix matrix) : IdentityMatrix(matrix.get_rows())
    {
        if (matrix.get_cols() != matrix.get_rows())
            throw runtime_error("[ERROR] [IdentityMatrix(Matrix)]: the dimensional problem occurred");
        for (int i = 0; i < matrix.get_rows(); i++)
            for (int j = 0; j < matrix.get_cols(); j++)
                this->matrix[i][j] = matrix[i][j];
    }
};

// Class for an elimination matrix that inherites the identity matrix class.
class EliminationMatrix : public IdentityMatrix
{
public:
    EliminationMatrix(size_t size, size_t i, size_t j, double val) : IdentityMatrix(size)
    {
        // Elimination matrix is an identity matrix, but with a special value at ij.
        matrix[i][j] = val * -1.0;
    }
    EliminationMatrix(Matrix matrix) : IdentityMatrix(matrix.get_rows())
    {
        if (matrix.get_cols() != matrix.get_rows())
            throw runtime_error("[ERROR] [EliminationMatrix(Matrix)]: the dimensional problem occurred");
        for (int i = 0; i < matrix.get_rows(); i++)
            for (int j = 0; j < matrix.get_cols(); j++)
                this->matrix[i][j] = matrix[i][j];
    }
};

// Class for a permutation matrix that inherites the identity matrix class.
class PermutationMatrix : public IdentityMatrix
{
public:
    PermutationMatrix(size_t size, size_t i, size_t j) : IdentityMatrix(size)
    {
        // Permutation matrix is a matrix that has rows i and j exchanged.
        swap(matrix[i], matrix[j]);
    }
    PermutationMatrix(Matrix matrix) : IdentityMatrix(matrix.get_rows())
    {
        if (matrix.get_cols() != matrix.get_rows())
            throw runtime_error("[ERROR] [PermutationMatrix(Matrix)]: the dimensional problem occurred");
        for (int i = 0; i < matrix.get_rows(); i++)
            for (int j = 0; j < matrix.get_cols(); j++)
                this->matrix[i][j] = matrix[i][j];
    }
};

// Class for a column vector that inherites the general matrix class.
class ColumnVector : public Matrix
{
public:
    // Constructor.
    ColumnVector() {}
    ColumnVector(size_t size) : Matrix(size, 1) {}
    ColumnVector(Matrix matrix) : ColumnVector(matrix.get_rows())
    {
        if (matrix.get_cols() != 1)
            throw runtime_error("[ERROR] [ColumnVector(Matrix)]: the dimensional problem occurred");
        for (int i = 0; i < matrix.get_rows(); i++)
            this->matrix[i][0] = matrix[i][0];
    }
    using Matrix::operator=;
    // + operator overloading using downcasting and upcasting.
    ColumnVector operator+(const ColumnVector &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) + (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const ColumnVector *C_ptr = (ColumnVector *)(upcast_C_ptr);

        ColumnVector C(cols);
        C = *C_ptr;

        return C;
    }
    // - operator overloading using downcasting and upcasting.
    ColumnVector operator-(const ColumnVector &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) - (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const ColumnVector *C_ptr = (ColumnVector *)(upcast_C_ptr);

        ColumnVector C(cols);
        C = *C_ptr;

        return C;
    }
    // * operator overloading using downcasting and upcasting.
    ColumnVector operator*(const ColumnVector &B) const
    {
        const Matrix *upcast_A = this;
        const Matrix *upcast_B = &B;

        const Matrix upcast_C = (*upcast_A) * (*upcast_B);
        const Matrix *upcast_C_ptr = &upcast_C;

        const ColumnVector *C_ptr = (ColumnVector *)(upcast_C_ptr);

        ColumnVector C(cols);
        C = *C_ptr;

        return C;
    }
    // Transpose method overloading using downcasting and upcasting.
    ColumnVector T() const
    {
        const Matrix *upcast_A = this;

        const Matrix upcast_C = upcast_A->T();
        const Matrix *upcast_C_ptr = &upcast_C;

        const ColumnVector *C_ptr = (ColumnVector *)(upcast_C_ptr);

        ColumnVector C(cols);
        C = *C_ptr;

        return C;
    }
    // Method to get norm of vector.
    double norm()
    {
        double result = 0.0;
        for (size_t i = 0; i < rows; i++)
            result += matrix[i][0] * matrix[i][0];
        return sqrt(result);
    }
};

// Class for storing an augmented matrix with different types of left and right matrices.
template <typename L, typename R>
class AugmentedMatrix
{
protected:
    L matrixLeft;
    R matrixRight;

public:
    // Constructor.
    AugmentedMatrix(const AugmentedMatrix<L, R> &old)
    {
        matrixLeft = old.matrixLeft;
        matrixRight = old.matrixRight;
    }
    AugmentedMatrix(L A, R B)
    {
        if (A.get_rows() != B.get_rows())
            throw runtime_error("[ERROR] [AugmentedMatrix(L, R)]: the dimensional problem occurred");
        matrixLeft = A;
        matrixRight = B;
    }
    // Getter for left matrix.
    L &getLeft() { return matrixLeft; }
    // Getter for right matrix.
    R &getRight() { return matrixRight; }
    AugmentedMatrix<L, R> &operator=(const AugmentedMatrix<L, R> &old)
    {
        if (this == &old)
            return *this;

        matrixLeft = old.matrixLeft;
        matrixRight = old.matrixRight;

        return *this;
    }
    // Cout operator overloading.
    friend ostream &operator<<(ostream &out, AugmentedMatrix &m)
    {
        out << m.matrixLeft << m.matrixRight;
        return out;
    }
    // Method to execute forward elimination.
    friend AugmentedMatrix<L, R> ForwardElimination(AugmentedMatrix<L, R> U, bool debug_info, int &no_perms)
    {
        // This will be our final augmented matrix stroing the upper triangle.
        size_t curr_col = 0;
        size_t rows = U.matrixLeft.get_rows();
        // Iterate through the rows.
        for (size_t i = 0; i < rows; i++)
        {
            int row_with_max_pivot = -1;
            // Find the maximum pivot.
            double max_pivot = U.matrixLeft[i][curr_col];
            for (size_t j = i + 1; j < rows; j++)
            {
                if (abs(U.matrixLeft[j][curr_col]) < EPS)
                    continue;
                if (abs(U.matrixLeft[j][curr_col]) > abs(max_pivot))
                {
                    row_with_max_pivot = j;
                    max_pivot = U.matrixLeft[j][curr_col];
                }
            }
            // If there is a maximum pivot, then we should swap row with it with the current row.
            if (row_with_max_pivot != -1)
            {
                if (debug_info)
                    cout << "step: permutation\n";
                PermutationMatrix P(rows, i, row_with_max_pivot);
                // To exchange the rows we multiply by permuatation matrix.
                U.matrixLeft = P * U.matrixLeft;
                U.matrixRight = P * U.matrixRight;
                if (debug_info)
                    cout << U.matrixLeft;
                no_perms++;
            }
            // Go through the next rows and eliminate the values in the column under the pivot.
            for (size_t j = i + 1; j < rows; j++)
            {
                if (abs(U.matrixLeft[j][curr_col]) < EPS or abs(U.matrixLeft[i][curr_col]) < EPS)
                    continue;
                if (debug_info)
                    cout << "step: elimination\n";
                double elimK = U.matrixLeft[j][curr_col] / U.matrixLeft[i][curr_col];
                if (abs(elimK) < EPS)
                    elimK = 0;
                EliminationMatrix E(rows, j, i, elimK);
                // Tho eliminate the value we multiply by elimination matrix.
                U.matrixLeft = E * U.matrixLeft;
                U.matrixRight = E * U.matrixRight;
                if (debug_info)
                    cout << U.matrixLeft;
            }
            curr_col++;
        }
        return U;
    }
    friend AugmentedMatrix<L, R> BackwardElimination(AugmentedMatrix<L, R> U, bool debug_info)
    {
        int curr_col = U.matrixLeft.get_cols() - 1;
        int rows = U.matrixLeft.get_rows();
        for (int i = rows - 1; i >= 0; i--)
        {
            for (int j = i - 1; j >= 0; j--)
            {
                if (abs(U.matrixLeft[j][curr_col]) < EPS || abs(U.matrixLeft[i][curr_col]) < EPS)
                    continue;
                EliminationMatrix E(rows, j, i, U.matrixLeft[j][curr_col] / U.matrixLeft[i][curr_col]);
                U.matrixLeft = E * U.matrixLeft;
                U.matrixRight = E * U.matrixRight;
                if (debug_info)
                    cout << U;
            }
            curr_col--;
        }
        return U;
    }
    friend ColumnVector solve(AugmentedMatrix<SquareMatrix, ColumnVector> A, bool debug_info);
};

// Method to calculate the determinant.
double determinant_worker(SquareMatrix A, bool debug_info)
{
    AugmentedMatrix<SquareMatrix, SquareMatrix> aug(A, A);
    // The determinant can be calculated as a product
    // of values on main diagonal after forward elimination.
    int no_perms = 0;
    SquareMatrix U = ForwardElimination(aug, debug_info, no_perms).getLeft();
    double res = (no_perms % 2) ? -1.0 : 1.0;
    for (size_t i = 0; i < A.get_rows(); i++)
        res *= U[i][i];
    return res;
}

SquareMatrix inverse_worker(SquareMatrix A, bool debug_info)
{
    IdentityMatrix I(A.get_rows());
    // SquareMatrix I_matrix = I;
    if (debug_info)
        cout << "step #0: Augmented Matrix\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> aug(A, I);
    if (debug_info)
        cout << aug;

    if (debug_info)
        cout << "Direct way:\n";
    int no_perms = 0;
    AugmentedMatrix<SquareMatrix, SquareMatrix> B = ForwardElimination(aug, debug_info, no_perms);

    if (debug_info)
        cout << "Way back:\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> C = BackwardElimination(B, debug_info);

    if (debug_info)
        cout << "Diagonal normalization:\n";

    for (int i = 0; i < C.getLeft().get_rows(); i++)
    {
        double pivot = C.getLeft()[i][i];
        for (int j = 0; j < C.getLeft().get_rows(); j++)
            C.getLeft()[i][j] /= pivot;
        for (int j = 0; j < C.getRight().get_rows(); j++)
            C.getRight()[i][j] /= pivot;
    }
    if (debug_info)
        cout << C;

    return C.getRight();
}

void solve(AugmentedMatrix<SquareMatrix, ColumnVector> A, double error, bool debug_info)
{
    IdentityMatrix I(A.getLeft().get_rows());
    SquareMatrix D(A.getLeft().get_rows());
    for (int i = 0; i < D.get_rows(); i++)
        for (int j = 0; j < D.get_cols(); j++)
            D[i][j] = (i != j) ? 0 : A.getLeft()[i][j];
    SquareMatrix D_inv = D.inverse(false);

    SquareMatrix alpha = I - D_inv * A.getLeft();
    ColumnVector beta = Matrix(D_inv) * A.getRight();

    if (debug_info)
    {
        cout << "alpha:" << '\n';
        cout << alpha;
        cout << "beta:" << '\n';
        cout << beta;
    }

    ColumnVector x_old(beta.get_rows());
    ColumnVector x(beta.get_rows());
    x_old = beta;

    int it_count = 0;
    while (true)
    {
        if (debug_info)
        {
            cout << "x(" << it_count << "):" << '\n';
            cout << x_old;
        }

        x = Matrix(alpha) * x_old + beta;

        ColumnVector dx = Matrix(x) - Matrix(x_old);
        double curr_err = dx.norm();
        if (debug_info)
            cout << "e: " << curr_err << '\n';

        x_old = x;
        it_count++;

        if (curr_err < error)
            break;
    }

    if (debug_info)
    {
        cout << "x(" << it_count << "):" << '\n';
        cout << x_old;
    }
}

int main(void)
{
    cout << fixed << setprecision(4);

    int A_n;
    cin >> A_n;
    SquareMatrix matrix(A_n);
    cin >> matrix;

    int b_n;
    cin >> b_n;
    ColumnVector column(b_n);
    cin >> column;

    double error;
    cin >> error;

    AugmentedMatrix<SquareMatrix, ColumnVector> A(matrix, column);

    // Check if Jacobi is applicable
    for (int i = 0; i < A_n; i++)
    {
        double row_sum = 0;
        for (int j = 0; j < A_n; j++)
            if (i != j)
                row_sum += matrix[i][j];
        double diag = matrix[i][i];
        if (abs(diag) < EPS or diag + EPS < row_sum)
        {
            cout << "The method is not applicable!" << '\n';
            return 0;
        }
    }

    solve(A, error, true);

    return 0;
}