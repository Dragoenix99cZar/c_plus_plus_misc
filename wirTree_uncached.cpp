#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <windows.h>

namespace fs = std::filesystem;

struct Entry {
    std::string name;
    std::string type;
    uintmax_t size;
};

std::string human_readable_size(uintmax_t size) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unit = 0;
    double readable = static_cast<double>(size);

    while (readable >= 1024 && unit < 4) {
        readable /= 1024;
        ++unit;
    }

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.2f %s", readable, units[unit]);
    return std::string(buffer);
}

uintmax_t get_directory_size(const fs::path& dir_path) {
    uintmax_t total = 0;

    try {
        for (auto& p : fs::recursive_directory_iterator(dir_path, fs::directory_options::skip_permission_denied)) {
            if (fs::is_regular_file(p)) {
                total += fs::file_size(p);
            }
        }
    } catch (...) {
        // skip unreadable directories
    }

    return total;
}

std::vector<Entry> scan_directory(const fs::path& path, uintmax_t& total_size) {
    std::vector<Entry> entries;
    total_size = 0;

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            try {
                std::string name = entry.path().filename().string();
                uintmax_t size = 0;
                std::string type;

                if (fs::is_directory(entry)) {
                    size = get_directory_size(entry);
                    type = "Folder";
                } else if (fs::is_regular_file(entry)) {
                    size = fs::file_size(entry);
                    type = "File";
                } else {
                    continue;
                }

                entries.push_back({name, type, size});
                total_size += size;

            } catch (...) {
                continue;
            }
        }
    } catch (...) {
        std::cerr << "Failed to scan path.\n";
    }

    return entries;
}

void print_table(const std::vector<Entry>& entries, uintmax_t total_size) {
    std::vector<Entry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const Entry& a, const Entry& b) {
        return a.size > b.size;
    });

    std::cout << std::left << std::setw(40) << "Name"
              << std::setw(10) << "Type"
              << std::setw(15) << "Size"
              << std::setw(12) << "% of Total" << "\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& e : sorted) {
        double percent = total_size ? (e.size * 100.0 / total_size) : 0.0;
        std::cout << std::left << std::setw(40) << e.name
                  << std::setw(10) << e.type
                  << std::setw(15) << human_readable_size(e.size)
                  << std::setw(10) << std::fixed << std::setprecision(2) << percent << "%" << "\n";
    }

    std::cout << "\nTotal Size: " << human_readable_size(total_size) << "\n";
}

int main(int argc, char* argv[]) {
    std::string input_path;
    // std::cout << "Enter folder path to scan: ";
    // std::getline(std::cin, input_path);

    input_path = argv[1];

    fs::path path(input_path);
    if (!fs::exists(path) || !fs::is_directory(path)) {
        std::cerr << "Invalid path.\n";
        return 1;
    }
    
    SetConsoleOutputCP(CP_UTF8); // To support emoji char
    std::cout << "\n📁 Scanning: " << path << " ...\n\n";
    uintmax_t total_size = 0;
    auto entries = scan_directory(path, total_size);
    print_table(entries, total_size);

    return 0;
}
