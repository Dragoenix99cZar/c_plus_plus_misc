// clang++ -O2 levenshtein.cpp -o mytool/diffw
// clang++ -Wall -Wextra -ggdb levenshtein.cpp -o levenshtein

#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <chrono>  // ⏱️ For timing
#include <windows.h> // For Emoji
#include <iomanip>


// Print the DP table for visualization
void print_dp_table(const std::string& a, const std::string& b,
    const std::vector<std::vector<int>>& dp,
    const std::vector<std::vector<char>>& op) {
    int n = a.size(), m = b.size();

    std::cout << "       ";
    for (char ch : b)
    {
        std::cout << std::setw(6) << ch;
    }
    std::cout << "\n";

    for (int i = 0; i <= n; ++i)
    {
        std::cout << (i > 0 ? a[i - 1] : ' ') << " ";
        for (int j = 0; j <= m; ++j)
        {
            char o = op[i][j];
            if (o == ' ') o = ' ';
            std::cout << std::setw(3) << dp[i][j] << '(' << o << ')';
        }
        std::cout << "\n";
    }
}



int levenshtein(const std::string& a, const std::string& b, std::vector<char>& edits) {
    int n = a.size(), m = b.size();
    std::vector<std::vector<int>> dp(n+1, std::vector<int>(m+1));
    std::vector<std::vector<char>> op(n + 1, std::vector<char>(m + 1)); // tracks operations

    for (int i = 0; i <= n; ++i) {
        dp[i][0] = i;
        op[i][0] = 'D';
    }
    for (int j = 0; j <= m; ++j) {
        dp[0][j] = j;
        op[0][j] = 'I';
    }
    op[0][0] = ' '; // no operation

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
                op[i][j] = 'M';
            } else {
                int del = dp[i - 1][j] + 1;
                int ins = dp[i][j - 1] + 1;
                int sub = dp[i - 1][j - 1] + 1;

                dp[i][j] = std::min({ del, ins, sub });

                if (dp[i][j] == sub) op[i][j] = 'S';
                else if (dp[i][j] == del) op[i][j] = 'D';
                else op[i][j] = 'I';
            }
        }
    }

    // Traceback to collect edits
    int i = n, j = m;
    while (i > 0 || j > 0) {
        char action = op[i][j];
        edits.push_back(action);
        if (action == 'M' || action == 'S') {
            --i; --j;
        } else if (action == 'D') {
            --i;
        } else if (action == 'I') {
            --j;
        }
    }
    std::reverse(edits.begin(), edits.end());
    
    print_dp_table(a, b, dp, op); // Show table

    return dp[n][m];
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <word1> <word2>\n";
        std::cerr << "   eg: " << argv[0] << " \"pineapple\" \"anananas\" \n";
        return 1;
    }
    // ⏱️ Start timing
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<char> edits;
    std::string text1 = argv[1];
    std::string text2 = argv[2];
    int dist = levenshtein(text1, text2, edits);

    std::cout << "\nLevenshtein distance: " << dist << "\n";
    std::cout << "               ";
    for (char op : text1) std::cout << op << " ";
    std::cout << "\n";
    std::cout << "Edit sequence: ";
    for (char op : edits) std::cout << op << " ";
    std::cout << "\n";
    std::cout << "               ";
    for (char op : text2) std::cout << op << " ";
    std::cout << "\n";

    // ⏱️ End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n";
    std::cout << "⏱️ Execution time: " << elapsed_seconds.count() << " seconds\n";

    return EXIT_SUCCESS;
}