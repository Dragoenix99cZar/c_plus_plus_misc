// clang++ -std=c++17 -Wall -Wextra -ggdb codebaseAnalyzer.cpp -o mytool & codebaseStats
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <chrono>
#include <algorithm> // std::count
#include <windows.h> // For Emoji

namespace fs = std::filesystem;

// ANSI color macros
#define GREEN   "\033[1;32m"
#define CYAN    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[0;37m"
#define BWHITE  "\033[1;37m"
#define RESET   "\033[0m"

// Global stats
size_t longest_filename_length = 3;
size_t longest_file_ext_length = 3;
int totalLinesOfCode = 0;
long long totalSrcFileSize = 0;

// Extensions to scan
std::vector<std::string> extensions = { ".cpp", ".c", ".py", ".cs", ".h", ".hpp", ".ps1", ".js", ".java"};
std::vector<std::string> excludeExtensions;
std::unordered_set<std::string> notRecognizedFiles;


// Check if file has a valid extension
bool hasValidExtension(const fs::path& path) {
    std::string ext = path.extension().string();
    for (const auto& validExt : excludeExtensions) {
        if (ext == validExt) {
            return false;
        }
    }
    for (const auto& validExt : extensions) {
        if (ext == validExt) {
            return true;
        }
    }
    if (ext.length() > longest_file_ext_length)
    {
        longest_file_ext_length = ext.length();
    }
    notRecognizedFiles.insert(path.extension().string());
    return false;
}

// Count lines of code in file
int countLines(const fs::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return 0;
    int count = 0;
    std::string line;
    while (std::getline(file, line)) ++count;
    totalLinesOfCode += count;
    return count;
}

// Get file size
long long getFileSize(const fs::path& path) {
    try {
        auto size = fs::file_size(path);
        totalSrcFileSize += size;
        return size;
    }
    catch (...) {
        return 0;
    }
}

// Human-readable size
std::string formatSize(long long size) {
    constexpr const char* units[] = { "B", "KB", "MB", "GB" };
    int i = 0;
    double s = static_cast<double>(size);
    while (s >= 1024 && i < 3) {
        s /= 1024;
        ++i;
    }
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << s << " " << std::setw(3) << units[i];
    return oss.str();
}

// Traverse directory
void traverse(const fs::path& dir, std::unordered_map<std::string, std::vector<std::string>>& fileData)
{
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            const auto& path = entry.path();
            if (hasValidExtension(path)) {
                std::string fullPath = path.string();
                std::string fileName = path.filename().string();

                if (fileName.size() > longest_filename_length)
                {
                    longest_filename_length = fileName.size();
                }

                int lines = countLines(path);
                long long size = getFileSize(path);

                fileData[fullPath] = { fileName, std::to_string(lines), formatSize(size) };

            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path> [<ext1> <ext2> ...] [--exclude <ext1> <ext2> ...] \n";
        std::cerr << "   eg: " << argv[0] << " . // current dir\n";
        std::cerr << "       " << argv[0] << " . .csv .txt\n";
        return EXIT_FAILURE;
    }

    // Custom extensions (optional)
    if (argc > 2)
    {
        /*for (int i = 2; i < argc; ++i) {
            extensions.emplace_back(argv[i]);
        }*/
        bool excludeMode = false;
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "--exclude" || arg == "-e") {
                excludeMode = true;
                continue;
            }

            if (excludeMode) {
                excludeExtensions.emplace_back(arg);
            }
            else {
                extensions.emplace_back(arg); // Additional extensions to include
            }
        }

    }

    std::string dirPath = argv[1];
    std::unordered_map<std::string, std::vector<std::string>> fileData;


    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n🔍 Scanning directory: " << dirPath << "\n";

    auto start = std::chrono::high_resolution_clock::now();
    traverse(dirPath, fileData);
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();

    // Table headers
    std::cout << std::setw(longest_filename_length + 5) << "File"
              << std::setw(20) << "Lines"
              << std::setw(20) << "Size\n";
    std::cout << std::string(longest_filename_length + 45, '-') << "\n";

    start = std::chrono::high_resolution_clock::now();
    int fileCount = 0;
    bool colorSwitch = true;
    for (const auto& [fullPath, info] : fileData)
    {
        std::cout << ((colorSwitch)? BWHITE : YELLOW);
        std::cout << std::setw(longest_filename_length + 5) << std::right << info[0]    
                  << std::setw(20) << info[1]                                           
                  << std::setw(20) << info[2] << "\n" << RESET;                                  
        fileCount++;
        colorSwitch = !colorSwitch;
    }
    end = std::chrono::high_resolution_clock::now();


    std::cout << std::string(longest_filename_length + 45, '-') << "\n";
    std::cout << "📄 Total Files        : " << fileCount << "\n";
    std::cout << "📏 Total LOC          : " << totalLinesOfCode << "\n";
    std::cout << "💾 Total Size         : " << formatSize(totalSrcFileSize) << "\n";
    std::cout << "⏱️ Time Taken         : " << duration << " sec\n";
    std::cout << "⏱️ Print Time Taken   : " << (std::chrono::duration<double>(end - start).count()) << " sec\n";

    std::cout << "\n";
    std::cout << std::string(longest_filename_length + 45, '-') << "\n";
    std::cout << "Incuded extensions: ";
    for (const auto& ext : extensions) std::cout << ext << " ";
    std::cout << "\n";
    std::cout << "Excluded extensions: ";
    for (const auto& ext : excludeExtensions) std::cout << ext << " ";

    if (!notRecognizedFiles.empty()) {
        std::cout << "\nUnrecognized Files:\n";
        int e = 0;
        for (const auto& f : notRecognizedFiles) {
            std::cout << std::setw(longest_file_ext_length) << f << " | ";
            if (e++ >= 5)
            {
                std::cout << "\n";
                e = 0;
            }
        }
    }

    return EXIT_SUCCESS;
}
