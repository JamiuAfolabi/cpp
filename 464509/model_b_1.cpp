
#include<iostream>
using namespace std;

// Function to serialize an array of integers into a single integer
unsigned long long serialize(int arr[], int n)
{
    unsigned long long result = 0;

    for (int i = 0; i < n; i++)
    {
        // Check for integer overflow
        if (result > ((ULONG_LONG_MAX - arr[i]) >> 32))
            return -1; // Return -1 to indicate overflow
        result = (result << 32) | arr[i];
    }

    return result;
}

// Function to deserialize the single integer back to the array
void deserialize(unsigned long long serialized_int, int arr[], int n)
{
    for (int i = n - 1; i >= 0; i--)
    {
        arr[i] = (int)(serialized_int & 0xFFFFFFFF); // Extract the least significant 32 bits
        serialized_int >>= 32; // Right shift to remove the extracted bits
    }
}


int32_t main() {
	int n;
	cout << "Enter number of elements : ";
	cin >> n;
	int arr[n];
	
	cout << "Enter all elements separated by space : ";
	for (int i = 0; i < n; i++) {
		cin >> arr[i];
	}
 
    // Serialize the array
    unsigned long long serialized_data = serialize(arr, n);

    if (serialized_data == -1)
    {
        cout << "Integer overflow occurred during serialization." << endl;
    } 
    else
    {
        cout << "Serialized data: " << serialized_data << endl;
 
        // Deserialize the data back to the original array
        int arr_deserialized[n];
        deserialize(serialized_data, arr_deserialized, n);

        cout << "Deserialized Array: \n";
        for (int i = 0; i < n; i++)
        {
            cout << arr_deserialized[i] << " ";
        }
    }

	return 0;
}
  
