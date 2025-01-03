#include<iostream>
#include<iomanip>
#include<algorithm>
using namespace std;
float **a, ratio;
int i, j, k, n;

const auto& rowModify = [](float **a, int row1, int row2, int col, float scale)
                {
                    for (k = 0; k < 2 * n; k++) {
                        a[row1][k] += scale * a[row2][k];
                    }
                };

int main() {
    cout << setprecision(3) << fixed;
    cout << "Enter order of matrix: ";
    cin >> n;
    a = new float*[n]; for (i = 0; i < n; i++) a[i] = new float[2 * n];
    cout << "Enter coefficients of Matrix: " << endl;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> a[i][j];
        }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            if (i == j) a[i][j + n] = 1;
            else a[i][j + n] = 0;
        }
    for (i = 0; i < n; i++) {
        if (a[i][i] == 0.0) {
            cout << "Mathematical Error!" << endl;
            exit(0);
        }
        for (j = i + 1; j < n; j++) {
            ratio = a[j][i] / a[i][i];
            rowModify(a, j, i, i, -ratio); 
        }
    }
    for (i = 0; i < n; i++)
        for (j = 0; j < 2 * n; j++)
            a[i][j] = a[i][j] / a[i][i];

    cout << endl << "Inverse Matrix is:" << endl;
    for (i = 0; i < n; i++) {
        for (j = n; j < 2 * n; j++) {
            cout << a[i][j] << "\t";
        }
        cout << endl;
    }

    for (i = 0; i < n; i++)
        delete[] a[i];
    delete[] a;
    return 0;
}