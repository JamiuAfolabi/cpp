
#include <iostream>
#include <vector>

vector<double> iterateSignalMultiplication(double signal, int n)
{
    vector<double> result;

    if (n <= 0)
    {
        return result;
    }

    result.push_back(signal);

    for (int i = 1; i <= n; i++)
    {
        // Use bitwise left shift operator to multiply signal by 2
        signal *= 2.0; 
        result.push_back(signal);
    }

    return result;
}

int main() {
    std::cout << "Enter the signal value: ";
    double signal;
    std::cin >> signal;

    std::cout << "Enter the value of n: ";
    int n;
    std::cin >> n;

    vector<double> result = iterateSignalMultiplication(signal, n);
    
    if(result.empty())
    {
        std::cout << "Empty vector returned." << std::endl;
    }
    else
    {
        std::cout << "Resulting values stored in the vector are: " << std::endl;
        for (double val : result) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
                  