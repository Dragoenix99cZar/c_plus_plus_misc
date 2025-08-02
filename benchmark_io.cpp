// clang++ -std=c++17 -Wall -Wextra -ggdb -o benchmark_io benchmark_io.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <vector>
#include <numeric>
#include <cstdio>
#include <algorithm> // std::count

namespace fs = std::filesystem;

// Reset
#define RESET       "\033[0m"

// Regular Colors
#define BLACK       "\033[0;30m"
#define RED         "\033[0;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define BLUE        "\033[0;34m"
#define MAGENTA     "\033[0;35m"
#define CYAN        "\033[0;36m"
#define WHITE       "\033[0;37m"

// Bold Colors
#define BBLACK      "\033[1;30m"
#define BRED        "\033[1;31m"
#define BGREEN      "\033[1;32m"
#define BYELLOW     "\033[1;33m"
#define BBLUE       "\033[1;34m"
#define BMAGENTA    "\033[1;35m"
#define BCYAN       "\033[1;36m"
#define BWHITE      "\033[1;37m"

// Underline Colors
#define UBLACK      "\033[4;30m"
#define URED        "\033[4;31m"
#define UGREEN      "\033[4;32m"
#define UYELLOW     "\033[4;33m"
#define UBLUE       "\033[4;34m"
#define UMAGENTA    "\033[4;35m"
#define UCYAN       "\033[4;36m"
#define UWHITE      "\033[4;37m"

// Background Colors
#define ON_BLACK    "\033[40m"
#define ON_RED      "\033[41m"
#define ON_GREEN    "\033[42m"
#define ON_YELLOW   "\033[43m"
#define ON_BLUE     "\033[44m"
#define ON_MAGENTA  "\033[45m"
#define ON_CYAN     "\033[46m"
#define ON_WHITE    "\033[47m"

/*
If you're using a Windows console and the colors don’t show up: Enable ANSI escape codes with this snippet:
#include <windows.h>

void enableAnsiColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
Call enableAnsiColors() at the start of your program.
*/
struct FileBenchmark {
    std::string path;
    long long size_fseek = 0;
    long long size_fstream = 0;
    long long size_filesystem = 0;
    int lines_getline = 0;
    int lines_iterator = 0;
    long long time_fseek = 0;
    long long time_getline = 0;
    long long time_iterator = 0;
    long long time_fstream = 0;
    long long time_fs_size = 0;
};

// Using C stdio to get file size
long long get_file_size_fseek(const std::string& path) {
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    long long size = ftell(fp);
    fclose(fp);
    return size;
}

// Using C++ std::ifstream to count lines with std::getline()
int count_lines_getline(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) return 0;
    std::string line;
    int lines = 0;
    while (std::getline(file, line)) {
        ++lines;
    }
    return lines;
}

// Using istreambuf_iterator to count newline characters
int count_lines_iterator(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return std::count(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>(), '\n');
}

// File size using ifstream + seekg
long long get_file_size_fstream(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    return file.tellg();
}

// Extensions to scan
std::vector<std::string> extensions = { ".cpp", ".c", ".py", ".cs", ".h", ".hpp", ".ps1", ".js", ".java", "json"};

// Check if file has a valid extension
bool hasValidExtension(const fs::path& path)
{
    std::string ext = path.extension().string();
    for (const auto& validExt : extensions) {
        if (ext == validExt) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << RED << "Usage: " << argv[0] << " <directory_path>\n";
        return 1;
    }

    std::string directory = argv[1];
    std::vector<FileBenchmark> results;

    FileBenchmark benchTotal;
    benchTotal.size_fseek = 0;
    benchTotal.time_fseek = 0;

    benchTotal.size_fstream = 0;
    benchTotal.time_fstream = 0;

    benchTotal.size_filesystem = 0;
    benchTotal.time_fs_size = 0;

    benchTotal.lines_getline = 0;
    benchTotal.time_getline = 0;

    benchTotal.lines_iterator = 0;
    benchTotal.time_iterator = 0;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (!fs::is_regular_file(entry.path())) continue;
        if (!hasValidExtension(entry.path())) continue;

        FileBenchmark bench;
        bench.path = entry.path().string();

        // File size with fseek
        auto start = std::chrono::high_resolution_clock::now();
        bench.size_fseek = get_file_size_fseek(bench.path);
        auto end = std::chrono::high_resolution_clock::now();
        bench.time_fseek = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // File size with fstream
        start = std::chrono::high_resolution_clock::now();
        bench.size_fstream = get_file_size_fstream(bench.path);
        end = std::chrono::high_resolution_clock::now();
        bench.time_fstream = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        
        // File size with filesystem
        start = std::chrono::high_resolution_clock::now();
        bench.size_filesystem = fs::file_size(bench.path);
        end = std::chrono::high_resolution_clock::now();
        bench.time_fs_size = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Count lines using std::getline
        start = std::chrono::high_resolution_clock::now();
        bench.lines_getline = count_lines_getline(bench.path);
        end = std::chrono::high_resolution_clock::now();
        bench.time_getline = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        // Count lines using iterator
        start = std::chrono::high_resolution_clock::now();
        bench.lines_iterator = count_lines_iterator(bench.path);
        end = std::chrono::high_resolution_clock::now();
        bench.time_iterator = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        results.push_back(bench);

        benchTotal.size_fseek += bench.size_fseek;
        benchTotal.time_fseek += bench.time_fseek;

        benchTotal.size_fstream += bench.size_fstream;
        benchTotal.time_fstream += bench.time_fstream;

        benchTotal.size_filesystem += bench.size_filesystem;
        benchTotal.time_fs_size += bench.time_fs_size;
        
        benchTotal.lines_getline += bench.lines_getline;
        benchTotal.time_getline += bench.time_getline;
        
        benchTotal.lines_iterator += bench.lines_iterator;
        benchTotal.time_iterator += bench.time_iterator;
    }

    std::cout << CYAN << "\nBenchmark Results:\n"  << RESET;
    std::cout << std::left << std::setw(40) << "File"
              << std::right << std::setw(18) << "Size(fseek)"
              << std::setw(18) << "Size(ifstream)"
              << std::setw(18) << "Size(fs::file_size)"
              << std::setw(18) << "Lines(getline)"
              << std::setw(18) << "Lines(it)"
              << std::setw(18) << "T_fseek(us)"
              << std::setw(18) << "T_fs::file_size(us)"
              << std::setw(18) << "T_fs::fstream"
              << std::setw(18) << "T_getl(us)"
              << std::setw(18) << "T_iter(us)" << "\n";
              
              for (const auto& b : results) {
                  std::cout << std::left << std::setw(40) << b.path.substr(b.path.find_last_of("/\\") + 1)
                  << std::right << std::setw(18) << b.size_fseek
                  << std::setw(18) << b.size_fstream
                  << std::setw(18) << b.size_filesystem
                  << std::setw(18) << b.lines_getline
                  << std::setw(18) << b.lines_iterator
                  << std::setw(18) << b.time_fseek
                  << std::setw(18) << b.time_fs_size
                  << std::setw(18) << b.time_fstream
                  << std::setw(18) << b.time_getline
                  << std::setw(18) << b.time_iterator << "\n";
                }
                
    std::cout << std::left << std::setw(40) << "File"
              << std::right << std::setw(18) << "Size(fseek)"
              << std::setw(18) << "Size(ifstream)"
              << std::setw(18) << "Size(fs::file_size)"
              << std::setw(18) << "Lines(getline)"
              << std::setw(18) << "Lines(it)"
              << std::setw(18) << "T_fseek(us)"
              << std::setw(18) << "T_fs::file_size(us)"
              << std::setw(18) << "T_fs::fstream"
              << std::setw(18) << "T_getl(us)"
              << std::setw(18) << "T_iter(us)" << "\n";
    
    std::cout << std::left << std::setw(40) << "-------Total: "
              << std::right << std::setw(18) << benchTotal.size_fseek
              << std::setw(18) << benchTotal.size_fstream
              << std::setw(18) << benchTotal.size_filesystem
              << std::setw(18) << benchTotal.lines_getline
              << std::setw(18) << benchTotal.lines_iterator
              << std::setw(18) << benchTotal.time_fseek
              << std::setw(18) << benchTotal.time_fs_size
              << std::setw(18) << benchTotal.time_fstream
              << std::setw(18) << benchTotal.time_getline
              << std::setw(18) << benchTotal.time_iterator << "\n";

    int count = results.size();
    std::cout << YELLOW << "\nAverage Times (microseconds):\n" << RESET;
    std::cout << "fseek     : " << GREEN << std::fixed << std::setprecision(2) << (benchTotal.time_fseek / count) << " us\n" << RESET;
    std::cout << "file_size : " << GREEN << (benchTotal.time_fs_size / count) << " us\n" << RESET;
    std::cout << "getline   : " << GREEN << (benchTotal.time_getline / count) << " us\n" << RESET;
    std::cout << "iterator  : " << GREEN << (benchTotal.time_iterator / count) << " us\n" << RESET;

    return 0;
}
