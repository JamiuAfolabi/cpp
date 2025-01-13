#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class BoyerMoore {
private:
    string pattern;
    int m;
    vector<int> bc; // bad character heuristic
    vector<int> gs; // good suffix heuristic

public:
    BoyerMoore(const string &p) : pattern(p) {
        m = pattern.length();
        bc.resize(256, -1); // bad character heuristic (ASCII character set)
        gs.resize(m + 1);
        preprocessBadCharacter();
        preprocessGoodSuffix();
    }

    void preprocessBadCharacter() {
        for (int i = 0; i < m; ++i) {
            bc[(unsigned char)pattern[i]] = i; // store last occurrence
        }
    }

    void preprocessGoodSuffix() {
        int lastPrefixPosition = m;
        for (int i = m; i > 0; --i) {
            if (isPrefix(i)) {
                lastPrefixPosition = i;
            }
            gs[m - i] = lastPrefixPosition - i + m;
        }
        for (int i = 0; i < m - 1; ++i) {
            int slen = suffixLength(i);
            gs[slen] = m - 1 - i + slen; // filling good suffix table
        }
    }

    bool isPrefix(int p) const {
        for (int i = p, j = 0; i < m; ++i, ++j) {
            if (pattern[i] != pattern[j]) return false;
        }
        return true;
    }

    int suffixLength(int p) const {
        int length = 0;
        for (int i = p, j = m - 1; i >= 0 && pattern[i] == pattern[j]; --i, --j) {
            ++length;
        }
        return length;
    }

    void search(const string &text) {
        int n = text.length();
        int skip;
        for (int i = 0; i <= n - m;) {
            skip = 0;
            for (int j = m - 1; j >= 0; --j) {
                if (pattern[j] != text[i + j]) {
                    skip = max(1, j - bc[(unsigned char)text[i + j]]);
                    skip = max(skip, gs[j]);
                    break;
                }
            }
            if (skip == 0) {
                cout << "Pattern found at index " << i << endl;
                skip = gs[0]; // move the pattern based on good suffix
            }
            i += skip;
        }
    }
};

int main() {
    string text = "ABABDABACDABABCABAB";
    string pattern = "ABABCABAB";
    BoyerMoore bm(pattern);
    bm.search(text);
    
    return 0;
}