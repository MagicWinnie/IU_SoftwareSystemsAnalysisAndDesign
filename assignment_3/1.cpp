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
    // Destructor.
    ~Matrix()
    {
        for (size_t i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;
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
    // = operator overloading.
    Matrix &operator=(Matrix &old)
    {
        if (this == &old)
            return *this;

        // First delete.
        for (size_t i = 0; i < rows; i++)
            delete[] matrix[i];
        delete[] matrix;

        rows = old.rows;
        cols = old.cols;

        // Then create.
        matrix = new double *[rows];
        for (size_t i = 0; i < rows; i++)
            matrix[i] = new double[cols];

        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                matrix[i][j] = old[i][j];

        return *this;
    }
    // + operator overloading.
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
    // - operator overloading.
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
    // * operator overloading.
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
    // Method to transpose the matrix.
    Matrix *T()
    {
        Matrix *B = new Matrix(cols, rows);
        for (size_t i = 0; i < rows; i++)
            for (size_t j = 0; j < cols; j++)
                B->matrix[j][i] = matrix[i][j];
        return B;
    }
};

// Class for a square matrix that inherites the general matrix class.
class SquareMatrix : public Matrix
{
protected:
    // Number of rows of square matrix equals number of columns.
    size_t size;
    // Method to get the inverse of the matrix.
    friend SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info);
    // Method to get the determinant of the matrix.
    friend double determinant_worker(SquareMatrix *A, bool debug_info);

public:
    // Constructor.
    SquareMatrix(size_t size) : Matrix(size, size) { this->size = size; }
    // Getter for the size of the matrix.
    size_t get_size() const { return size; }
    // + operator overloading using downcasting and upcasting.
    SquareMatrix *operator+(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) + (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    // - operator overloading using downcasting and upcasting.
    SquareMatrix *operator-(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) - (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    // * operator overloading using downcasting and upcasting.
    SquareMatrix *operator*(SquareMatrix &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) * (*upcast_B);
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    // Transpose method overloading using downcasting and upcasting.
    SquareMatrix *T()
    {
        Matrix *upcast_A = this;
        Matrix *upcast_C = (*upcast_A).T();
        SquareMatrix *C = (SquareMatrix *)(upcast_C);
        return C;
    }
    // Method to get the determinant of the matrix.
    double determinant(bool debug_info) { return determinant_worker(this, debug_info); }
    // Method to get the inverse of the matrix.
    SquareMatrix *inverse(bool debug_info) { return inverse_worker(this, debug_info); }
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
};

// Class for a column vector that inherites the general matrix class.
class ColumnVector : public Matrix
{
public:
    // Constructor.
    ColumnVector(size_t size) : Matrix(size, 1) {}
    // + operator overloading using downcasting and upcasting.
    ColumnVector *operator+(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) + (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    // - operator overloading using downcasting and upcasting.
    ColumnVector *operator-(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) - (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    // * operator overloading using downcasting and upcasting.
    ColumnVector *operator*(ColumnVector &B)
    {
        Matrix *upcast_A = this;
        Matrix *upcast_B = &B;
        Matrix *upcast_C = (*upcast_A) * (*upcast_B);
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    // Transpose method overloading using downcasting and upcasting.
    ColumnVector *T()
    {
        Matrix *upcast_A = this;
        Matrix *upcast_C = (*upcast_A).T();
        ColumnVector *C = (ColumnVector *)(upcast_C);
        return C;
    }
    // Method to get norm of vector.
    double norm()
    {
        double result = 0.0;
        for (size_t i = 0; i < rows; i++)
            result += i * i;
        return sqrt(result);
    }
};

// Class for storing an augmented matrix with different types of left and right matrices.
template <typename L, typename R>
class AugmentedMatrix
{
protected:
    L *matrixLeft;
    R *matrixRight;

public:
    // Constructor.
    AugmentedMatrix(L *A, R *B)
    {
        if (A->get_rows() != B->get_rows())
            throw runtime_error("Error: the dimensional problem occurred");
        matrixLeft = A;
        matrixRight = B;
    }
    // Destructor.
    ~AugmentedMatrix()
    {
        delete[] matrixLeft;
        delete[] matrixRight;
    }
    // Getter for left matrix.
    L *getLeft() const { return matrixLeft; }
    // Getter for right matrix.
    R *getRight() const { return matrixRight; }
    // Cout operator overloading.
    friend ostream &operator<<(ostream &out, AugmentedMatrix &m)
    {
        out << *m.getLeft() << *m.getRight();
        return out;
    }
    // Method to execute forward elimination.
    AugmentedMatrix<L, R> *ForwardElimination(bool debug_info, int *last_step)
    {
        // This will be our final augmented matrix stroing the upper triangle.
        AugmentedMatrix<L, R> *U = this;
        size_t curr_col = 0;
        size_t rows = matrixLeft->get_rows();
        // Iterate through the rows.
        for (size_t i = 0; i < rows; i++)
        {
            int row_with_max_pivot = -1;
            // Find the maximum pivot.
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
            // If there is a maximum pivot, then we should swap row with it with the current row.
            if (row_with_max_pivot != -1)
            {
                if (debug_info)
                    cout << "step #" << *last_step << ": permutation\n";
                PermutationMatrix *P = new PermutationMatrix(rows, i, row_with_max_pivot);
                // To exchange the rows we multiply by permuatation matrix.
                U->matrixLeft = (L *)((*(Matrix *)P) * (*(Matrix *)U->matrixLeft));
                U->matrixRight = (R *)((*(Matrix *)P) * (*(Matrix *)U->matrixRight));
                if (debug_info)
                {
                    cout << *U;
                    (*last_step)++;
                }
            }
            // Go through the next rows and eliminate the values in the column under the pivot.
            for (size_t j = i + 1; j < rows; j++)
            {
                if (abs((*U->matrixLeft)[j][curr_col]) < EPS or abs((*U->matrixLeft)[i][curr_col]) < EPS)
                    continue;
                if (debug_info)
                    cout << "step #" << *last_step << ": elimination\n";
                EliminationMatrix *E = new EliminationMatrix(rows, j, i, (*U->matrixLeft)[j][curr_col] / (*U->matrixLeft)[i][curr_col]);
                // Tho eliminate the value we multiply by elimination matrix.
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
    // Method to execute backward elimination.
    AugmentedMatrix<L, R> *BackwardElimination(bool debug_info, int *last_step)
    {
        AugmentedMatrix<L, R> *U = this;
        size_t curr_col = matrixLeft->get_cols() - 1;
        size_t rows = matrixLeft->get_rows();
        // Iterate through the rows.
        for (size_t i = rows; i >= 1; i--)
        {
            for (size_t j = i; j >= 1; j--)
            {
                if (abs((*U->matrixLeft)[j - 1][curr_col]) < EPS or abs((*U->matrixLeft)[i - 1][curr_col]) < EPS)
                    continue;
                if (debug_info)
                    cout << "step #" << *last_step << ": elimination\n";
                EliminationMatrix *E = new EliminationMatrix(rows, j - 1, i - 1, (*U->matrixLeft)[j - 1][curr_col] / (*U->matrixLeft)[i - 1][curr_col]);
                // Eliminate the values that are upper than the pivot by multiplying the elimination matrix.
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
    // Method to solve Ax=b.
    friend ColumnVector *solve(AugmentedMatrix<SquareMatrix, ColumnVector> *A, bool debug_info);
};

// Method to calculate the determinant.
double determinant_worker(SquareMatrix *A, bool debug_info)
{
    int step = 0;
    AugmentedMatrix<SquareMatrix, SquareMatrix> *aug = new AugmentedMatrix(A, A);
    // The determinant can be calculated as a product
    // of values on main diagonal after forward elimination.
    SquareMatrix *U = aug->ForwardElimination(debug_info, &step)->getLeft();
    double res = 1.0;
    for (size_t i = 0; i < A->get_size(); i++)
        res *= U->matrix[i][i];
    return res;
}

// Method to calculate the inverse.
SquareMatrix *inverse_worker(SquareMatrix *A, bool debug_info)
{
    // Create the augmented matrix: [A, I].
    IdentityMatrix *I = new IdentityMatrix(A->get_size());
    SquareMatrix *I_matrix = I;
    if (debug_info)
        cout << "step #0: Augmented Matrix\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *aug = new AugmentedMatrix(A, I_matrix);
    if (debug_info)
        cout << *aug;

    // Get the upper triangle in the left using forward elimination.
    int last_step = 1;
    if (debug_info)
        cout << "Direct way:\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *B = aug->ForwardElimination(debug_info, &last_step);

    // Get the D matrix in the left using backward elimination.
    if (debug_info)
        cout << "Way back:\n";
    AugmentedMatrix<SquareMatrix, SquareMatrix> *C = B->BackwardElimination(debug_info, &last_step);

    // Get the I matrix in the left by diving by the pivot value.
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

    // Get the left matrix to D.
    int last_step = 1;
    AugmentedMatrix<SquareMatrix, ColumnVector> *B = A->ForwardElimination(debug_info, &last_step);
    AugmentedMatrix<SquareMatrix, ColumnVector> *C = B->BackwardElimination(debug_info, &last_step);

    // Get the left matrix to I by dividing by the pivot.
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
    // Check for inconsistent system.
    // If zero rows in the left matrix exist then it is either NO or INF.
    // If zero row in the left matrix has non zero value in the right then
    // the system has no solution else infinite.
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

// Function to read matrices.
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
        // I throw runtime errors to indicate NO or INF.
        // So just print int.
        cout << e.what();
    }

    return 0;
}