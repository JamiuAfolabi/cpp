
#include<bits/stdc++.h>
using namespace std;

void fisherYatesShuffle(int* arr, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);  //Generate a random index 'j' between 0 and i
        swap(arr[i], arr[j]);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cout << "Enter the size of the array: ";
    cin >> n;
    int* arr = new int[n];

    cout << "Enter " << n << " elements separated by space: ";
    for (int i = 0; i < n; i++)
    {
        cin >> arr[i];
    }

    cout << "Original array: \n";
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";

    //seed random number generator
    srand(static_cast<unsigned>(time(0)));

    fisherYatesShuffle(arr, n);

    cout << "Shuffled array using Fisher-Yates: \n";
    for (int i = 0; i < n; i++)
    {
        cout << arr[i] << " ";
    }
    cout << "\n";

    //Freeing the memory
    delete[] arr;
    return 0;
}