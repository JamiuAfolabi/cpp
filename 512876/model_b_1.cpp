

#include<iostream>
using namespace std;

#define OUTLIER_THRESHOLD 3.0

double calculateMean(int array[], int size)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum / size;
}

double calculateStandardDeviation(int array[], int size, double mean)
{
    double sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += pow((array[i] - mean), 2);
    }
    return sqrt(sum / size);
}

bool isOutlier(int array[], int size)
{
    double mean = calculateMean(array, size);
    double stdDev = calculateStandardDeviation(array, size, mean);

    for (int i = 0; i < size; i++)
    {
        double zScore = (array[i] - mean) / stdDev;
        if (fabs(zScore) > OUTLIER_THRESHOLD)
        {
            cout << "Value " << array[i] << " is an outlier with z-score: " << zScore << endl;
            return true;
        }
    }
    cout << "No outliers found." << endl;
    return false;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cout << "Enter the size of the array: ";
    cin >> n;

    int arr[n];
    cout << "Enter the elements separated by space: ";
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    isOutlier(arr, n);

    return 0;
}
  