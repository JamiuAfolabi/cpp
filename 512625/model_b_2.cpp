#include <unordered_set>

// Function to find all pattern matches in the text using an unordered_set
void findPatternMatches(string text, string pattern, std::unordered_set<int>& matchPositions) {
   int patternLength = pattern.size(); 
   int textLength = text.size();
   int suffixArray[patternLength + 1];
   int shiftArray[patternLength + 1];

   // Initialize shift array to 0
   for (int i = 0; i <= patternLength; i++) {
      shiftArray[i] = 0;
   }

   // Compute the prefix and suffix shifts
   computePrefixShift(shiftArray, suffixArray, pattern);
   computeSuffixShift(shiftArray, suffixArray, pattern);

   int shift = 0;

   // Perform pattern matching using shifts
   while (shift <= (textLength - patternLength)) {
      int j = patternLength - 1;
      while (j >= 0 && pattern[j] == text[shift + j]) {
         j--; // Continue if characters match
      }
      if (j < 0) {
         matchPositions.insert(shift); // Store match position in the unordered_set
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

   std::unordered_set<int> matchPositions;

   // Find all pattern matches in the text
   findPatternMatches(text, pattern, matchPositions);

   // Display the positions of pattern matches
   for (int i : matchPositions) {
      cout << "Pattern found at position: " << i << endl;
   }

   return 0;
}