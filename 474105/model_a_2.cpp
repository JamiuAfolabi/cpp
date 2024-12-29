#include <iostream>
#include <vector>
using namespace std;

vector<double> iterateSignalMultiplication(double signal, int n)
{
    vector<double> result;

    // Special case: If n is 0 or negative, return an empty vector
    if (n <= 0)
    {
        return result;
    }

    result.push_back(signal);

    for (int i = 1; i <= n; i++)
    {
        // Using bitwise shift to multiply by 2
        signal = static_cast<double>(static_cast<int>(signal) << 1);
        result.push_back(signal);
    }

    return result;
}

int main() {
    double signal;
    int n;
    
    cout << "Enter the signal value: ";
    cin >> signal;
    
    cout << "Enter the value of n: ";
    cin >> n;

    vector<double> result = iterateSignalMultiplication(signal, n);

    if(result.empty())
    {
        cout << "Empty vector returned." << endl;
    }
    else
    {
        cout << "Resulting values stored in the vector are: " << endl;
        for (double val : result) {
            cout << val << " ";
        }
        cout << endl;
    }

    return 0;
}