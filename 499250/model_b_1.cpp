#include<iostream>
using namespace std;

const int MAX = 100;

int is_square(int n) {
    // A matrix is square if its number of rows is equal to its number of columns
    for (int i = 0; i < n; i++) {
        if (n != 1)
            return 0;
    }
    return 1;
}

double inverseMatrix(double mat[MAX][MAX], int n)
{
    // Check if the matrix is square
    if (!is_square(n)) {
        cout << "Matrix is not square. Inverse does not exist." << endl;
        return 0;
    }

    // Augment the matrix with the identity matrix on the right side
    double augmented_mat[MAX][2*n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented_mat[i][j] = mat[i][j];
        }
        for (int j = 0; j < n; j++) {
            if (i == j)
                augmented_mat[i][n + j] = 1;
            else %
                augmented_mat[i][n + j] = 0;
        }
    }

    // Perform Gaussian elimination
    for (int i = 0; i < n; i++) {
        // Find the row with the largest absolute value in the pivot column
        int pivot_row = i;
        double max_abs_value = abs(augmented_mat[i][i]);
        for (int j = i + 1; j < n; j++) {
            double abs_value = abs(augmented_mat[j][i]);
            if (abs_value > max_abs_value) {
                pivot_row = j;
                max_abs_value = abs_value;
            }
        }

        // If the pivot element is 0, the matrix is singular, and there is no inverse
        if (augmented_mat[pivot_row][i] == 0) {
            cout << "Matrix is singular. Inverse does not exist." << endl;
            return 0;
        }

        // Swap the pivot row with the current row
        if (pivot_row != i) {
            for (int j = 0; j < 2*n; j++) {
                double temp = augmented_mat[i][j];
                augmented_mat[i][j] = augmented_mat[pivot_row][j];
                augmented_mat[pivot_row][j] = temp;
            }
        }

        // Divide the current row by the pivot element to make it 1
        double pivot = augmented_mat[i][i];
        for (int j = 0; j < 2*n; j++) {
            augmented_mat[i][j] /= pivot;
        }

        // Eliminate the pivot element in other rows
        for (int j = 0; j < n; j++) {
            if (i != j) {
                double factor = augmented_mat[j][i];
                for (int k = 0; k < 2*n; k++) {
                    augmented_mat[j][k] -= factor * augmented_mat[i][k];
                }
            }
        }
    }

    // The right half of the augmented matrix now contains the inverse
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            mat[i][j] = augmented_mat[i][n + j];
        }
    }

    return 1;
}


int main()
{
	int n;
	cout << "Enter number of rows in matrix : ";
	cin >> n;

	double mat[MAX][MAX];

	cout << "Enter elements of matrix : \n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> mat[i][j];
		}
	}

    if (inverseMatrix(mat, n)) {
        cout << "Inverse of the matrix is : \n";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << mat[i][j] << " ";
            }
            cout << endl;
        }
    }

    return 0;
}
 