#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <utility>

namespace fs = std::filesystem;

// Regex patterns
std::regex classRegex(R"(class\s+(\w+)\s*\{)");
std::regex methodRegex(R"((\w+)\s+(\w+)\s*\([^)]*\)\s*\{)");
std::regex functionRegex(R"((\w+)\s+(\w+)\s*\([^)]*\)\s*\{)");
std::regex variableRegex(R"((\w+)\s+(\w+)\s*[=;])");

struct FileData {
    std::vector<std::pair<std::string, int>> classes;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> methods;
    std::vector<std::pair<std::string, int>> functions;
    std::vector<std::pair<std::string, int>> variables;
};

std::unordered_map<std::string, FileData> fileIndex;

std::vector<std::string> extensions = {".cpp", ".c", ".py", ".cs", ".h", ".hpp", ".ps1"};

bool hasExtension(const std::string& filePath, const std::vector<std::string>& extensions) {
    for (const auto& ext : extensions) {
        if (filePath.size() >= ext.size() &&
            filePath.compare(filePath.size() - ext.size(), ext.size(), ext) == 0) {
            return true;
        }
    }
    return false;
}

void processFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << "\n";
        return;
    }

    std::string line;
    std::string currentClass;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        std::smatch classMatch;
        if (std::regex_search(line, classMatch, classRegex)) {
            std::string className = classMatch[1];
            fileIndex[filePath].classes.push_back({className, lineNumber});
            currentClass = className;
        }

        std::smatch methodMatch;
        if (std::regex_search(line, methodMatch, methodRegex)) {
            std::string methodName = methodMatch[2];
            if (!currentClass.empty()) {
                fileIndex[filePath].methods[currentClass].push_back({methodName, lineNumber});
            }
        }

        std::smatch functionMatch;
        if (std::regex_search(line, functionMatch, functionRegex)) {
            std::string functionName = functionMatch[2];
            fileIndex[filePath].functions.push_back({functionName, lineNumber});
        }

        std::smatch variableMatch;
        if (std::regex_search(line, variableMatch, variableRegex)) {
            std::string variableName = variableMatch[2];
            fileIndex[filePath].variables.push_back({variableName, lineNumber});
        }
    }

    file.close();
}

void traverseDirectory(const std::string& directoryPath) {
    try {
        for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                if (hasExtension(filePath, extensions)) {
                    processFile(filePath);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    }
}

void displayIndex() {
    for (const auto& entry : fileIndex) {
        std::cout << "File: " << entry.first << "\n";

        if (!entry.second.classes.empty()) {
            std::cout << "  Classes:\n";
            for (const auto& cls : entry.second.classes) {
                std::cout << "    Class: " << cls.first << " (Line: " << cls.second << ")\n";
            }
        }

        if (!entry.second.methods.empty()) {
            std::cout << "  Methods:\n";
            for (const auto& methodEntry : entry.second.methods) {
                std::cout << "    Class: " << methodEntry.first << "\n";
                for (const auto& method : methodEntry.second) {
                    std::cout << "      Method: " << method.first << " (Line: " << method.second << ")\n";
                }
            }
        }

        if (!entry.second.functions.empty()) {
            std::cout << "  Functions:\n";
            for (const auto& func : entry.second.functions) {
                std::cout << "    Function: " << func.first << " (Line: " << func.second << ")\n";
            }
        }

        if (!entry.second.variables.empty()) {
            std::cout << "  Variables:\n";
            for (const auto& var : entry.second.variables) {
                std::cout << "    Variable: " << var.first << " (Line: " << var.second << ")\n";
            }
        }

        std::cout << "\n";
    }
}

bool isDirectory(const std::string& path) {
    return fs::is_directory(path);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <directory_path_or_file>\n";
        return 1;
    }

    std::string path = argv[1];

    if (isDirectory(path)) {
        traverseDirectory(path);
    } else {
        processFile(path);
    }

    displayIndex();
    return 0;
}
