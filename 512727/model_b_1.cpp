#include<bits/stdc++.h>
using namespace std;

bool isPalindromeBinary(int number)
{
    if (number == 0)
    {
        return true;
    }
    
    if (number < 0 || (number != 0 && number % 2 == 0))
    {
        return false;
    }
    
    int temp = number;
    int reversedNumber = 0;
    
    while (temp != 0)
    {
        int last_bit = temp & 1;
        reversedNumber = reversedNumber * 2 + last_bit;
        temp >>= 1; // Right shift to examine the next bit
    }
    
    return (number == reversedNumber);
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n;
    cin >> n;
    if(isPalindromeBinary(n)){
       cout << "Yes";
    }
    else{
       cout << "No";
    }
    return 0;
}