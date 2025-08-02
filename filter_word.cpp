// clang++ -Wall -Wextra -ggdb filter_word.cpp -o filter_word

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <cctype>
#include <chrono>  // ⏱️ For timing
#include <windows.h> // For Emoji
#include <iomanip>

const size_t MIN_WORD_LENGTH = 5;

bool all_unique_chars(const std::string& str) {
    uint32_t mask = 0;

    for (char c : str) {
        if (c < 'a' || c > 'z') continue; // skip non-lowercase
        int bit = c - 'a';

        if (mask & (1u << bit)) {
            // Bit already set -> duplicate
            return false;
        }

        mask |= (1u << bit);
    }

    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <wordlist.txt>\n";
        std::cerr << "   eg: " << argv[0] << " ./words_alpha.txt\n";
        return 1;
    }
    // ⏱️ Start timing
    auto start = std::chrono::high_resolution_clock::now();

    std::string inFilename = argv[1];

    // std::ifstream infile("words.txt");
    std::ifstream infile(inFilename);
    std::ofstream outfile("filtered_words.txt");

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error: Failed to open input or output file.\n";
        return 1;
    }

    std::string line;
    std::unordered_set<char> unique_alphabets;

    int totalWordCount = 0;
    int filteredWordCount = 0;

    while (std::getline(infile, line)) {
        // line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        // line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);

        size_t length = line.length();
        if (length != MIN_WORD_LENGTH)
        { 
            totalWordCount++;
            continue;
        }


        
        unique_alphabets.clear();
        for (char ch : line) {
            // if (std::isalpha(ch)) {
            // }
            unique_alphabets.insert(std::tolower(ch));
        }
        if (unique_alphabets.size() == length)
        // if (all_unique_chars(line))
        {
            outfile << line << '\n';
            filteredWordCount++;
        }
    }

    infile.close();
    outfile.close();

    // std::cout << "\nUnique alphabet characters in file: ";
    // for (char ch : unique_alphabets) {
    //     std::cout << ch << ' ';
    // }
    std::cout << "\n";
    // std::cout << "Total unique     : " << std::setw(10) << unique_alphabets.size() << "\n";
    std::cout << "Total words      : " << std::setw(10) << totalWordCount << "\n";
    std::cout << "Filtered words   : " << std::setw(10) << filteredWordCount << "\n";

    // ⏱️ End timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    
    SetConsoleOutputCP(CP_UTF8);
    std::cout << "⏱️ Execution time: " << elapsed_seconds.count() << " seconds\n";
    // auto durationMilli = end.time_since_epoch() - start.time_since_epoch();
    // std::cout << "⏱️ Execution time: " << durationMilli.count() << " milliseconds\n";

    return EXIT_SUCCESS;
}
