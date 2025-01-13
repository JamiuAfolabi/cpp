#include<iostream>
using namespace std;

// Function to compute prefix shift array
void computePrefixShift(int shiftArray[], int suffixArray[], string pattern) {
   int patternLength = pattern.size(); 
   int i = patternLength;
   int j = patternLength + 1;
   suffixArray[i] = j;
   
   // Traverse through the pattern to compute prefix shifts
   while(i > 0) {
      while(j <= patternLength && pattern[i-1] != pattern[j-1]) {
         if(shiftArray[j] == 0) {
            shiftArray[j] = j - i; // Set shift for mismatch
         }
         j = suffixArray[j]; // Update j based on suffix array
      }
      i--;
      j--;
      suffixArray[i] = j; // Update suffix array
   }  
}

// Function to compute suffix shift array
void computeSuffixShift(int shiftArray[], int suffixArray[], string pattern) {
   int patternLength = pattern.size(); 
   int j = suffixArray[0];
   
   // Fill shift array based on suffix array
   for(int i = 0; i < patternLength; i++) {
      if(shiftArray[i] == 0) {
         shiftArray[i] = j; 
         if(i == j) {
            j = suffixArray[j]; // Update j if match
         }
      }
   }
}

// Function to find all pattern matches in the text
void findPatternMatches(string text, string pattern, int matchArray[], int *matchCount) {
   int patternLength = pattern.size(); 
   int textLength = text.size();
   int suffixArray[patternLength + 1];
   int shiftArray[patternLength + 1];
   
   // Initialize shift array to 0
   for(int i = 0; i <= patternLength; i++) {
      shiftArray[i] = 0;
   }
   
   // Compute the prefix and suffix shifts
   computePrefixShift(shiftArray, suffixArray, pattern); 
   computeSuffixShift(shiftArray, suffixArray, pattern); 
   
   int shift = 0;
   // Perform pattern matching using shifts
   while(shift <= (textLength - patternLength)) {
      int j = patternLength - 1;
      while(j >= 0 && pattern[j] == text[shift + j]) {
         j--; // Continue if characters match
      }
      if(j < 0) {
         (*matchCount)++; // Increment match count if pattern matches
         matchArray[*matchCount] = shift; // Store match position
         shift += shiftArray[0]; // Shift by the value in the shift array
      } else {
          shift += shiftArray[j + 1]; // Shift by the value for mismatch position
      }
   }
}

int main() {
   string text, pattern;
   
   // Input text and pattern to search for
   cout << "Enter the original string: ";
   cin >> text;
   cout << "Enter the pattern to search: ";
   cin >> pattern;
   
   int matchArray[text.size()]; 
   int matchCount = -1;
   
   // Find all pattern matches in the text
   findPatternMatches(text, pattern, matchArray, &matchCount); 
   
   // Display the positions of pattern matches
   for(int i = 0; i <= matchCount; i++) {
      cout << "Pattern found at position: " << matchArray[i] << endl;
   }
}
