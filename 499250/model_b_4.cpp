#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using namespace std;

// Maybe monad for optional value handling
template <typename T>
class Maybe {
public:
    T value;
    bool hasValue;

    Maybe(T val) : value(val), hasValue(true) {}
    Maybe() : hasValue(false) {}

    // Retrieve value (may throw an exception if there is no value)
    T get() const {
        if (!hasValue) {
            throw runtime_error("No value present.");
        }
        return value;
    }

    // Map function to apply a function to the value if it exists
    Maybe<T> map(function<T(T)> f) const {
        if (!hasValue) {
            return Maybe<T>();
        }
        return Maybe<T>(f(value));
    }

    // FlatMap function to chain Maybe computations
    Maybe<T> flatMap(function<Maybe<T>(T)> f) const {
        if (!hasValue) {
            return Maybe<T>();
        }
        return f(value);
    }
};

int main() {
    vector<vector<float>> a;
    int n;

    cout << setprecision(3) << fixed;

    cout << "Enter order of matrix: ";
    cin >> n;

    a.resize(n, vector<float>(2 * n, 0));

    cout << "Enter coefficients of Matrix: " << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "a[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> a[i][j];
        }
    }

    for (int i = 0; i < n; i++) {
        a[i][i + n] = 1; // Identity matrix
    }

    auto scaleRow = [&a](int row, float scale) -> Maybe<void> {
        if (scale == 0) {
            return Maybe<void>(); // Error, cannot scale by zero
        }
        for (size_t j = 0; j < a[row].size(); j++) {
            a[row][j] /= scale;
        }
        return Maybe<void>();
    };

    auto addRows = [&a](int targetRow, int sourceRow, float ratio) -> Maybe<void> {
        for (size_t j = 0; j < a[targetRow].size(); j++) {
            a[targetRow][j] -= ratio * a[sourceRow][j];
        }
        return Maybe<void>();
    };

    auto gaussJordanElimination = [] (vector<vector<float>>& a) -> Maybe<void> {
        for (int i = 0; i < n; i++) {
            // Check for singular matrix
            float pivot = fabs(a[i][i]);
            if (pivot < 1e-9) {
                return Maybe<void>();
            }

            // Scale the pivot row
            Maybe<void> ret = scaleRow(i, pivot);
            if (!ret.hasValue()) {
                return ret; // Error encountered during scaling
            }

            // Eliminate below the pivot
            for (int j = i + 1; j < n; j++) {
                float ratio = a[j][i];
                Maybe<void> res = addRows(j, i, -ratio);
                if (!res.hasValue()) {
                    return res; // Error encountered during row addition
                }
            }
        }
        return Maybe<void>();
    };

    Maybe<void> computationResult = gaussJordanElimination(a);
    if (!computationResult.hasValue()) {
        cout << "Error: Division by zero or singular matrix." << endl;
    } else {
        // Display inverse matrix
        cout << endl << "Inverse Matrix is:" << endl;
        for (int i = 0; i < n; i++) {
            for (int j = n; j < 2 * n; j++) {
                cout << a[i][j] << "\t";
            }
            cout << endl;
        }
    }

    return 0;
}