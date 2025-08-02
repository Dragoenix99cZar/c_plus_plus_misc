#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <unordered_map>
#include <windows.h>
#include <chrono>
#include "TinySHA1.hpp"
#include "json.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

struct Entry {
    std::string name;
    std::string type;
    uintmax_t size;
    std::string sha1;
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

std::string compute_sha1_file(const fs::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return "";
    sha1::SHA1 sha1;
    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
        sha1.processBytes(buffer, file.gcount());
    }
    unsigned int digest[2];
    sha1.getDigest(digest);
    std::ostringstream result;
    for (int i = 0; i < 5; ++i)
        result << std::hex << std::setw(8) << std::setfill('0') << digest[i];
    return result.str();
}

json load_hashmap(const fs::path& path) {
    fs::path file = path / "0hashmap.txt";
    json j;
    if (fs::exists(file)) {
        std::ifstream in(file);
        std::string line;
        while (std::getline(in, line)) {
            std::istringstream ss(line);
            std::string full_path, size_str, sha1;
            std::getline(ss, full_path, ',');
            std::getline(ss, size_str, ',');
            std::getline(ss, sha1, ',');
            sha1.erase(std::remove(sha1.begin(), sha1.end(), '\\'), sha1.end());
            sha1.erase(std::remove(sha1.begin(), sha1.end(), '\"'), sha1.end());
            j[full_path] = { {"size", std::stoull(size_str)}, {"sha1", sha1} };
        }
    }
    return j;
}

void save_hashmap(const fs::path& path, const json& j) {
    std::ofstream out(path / "0hashmap.txt");
    for (auto& [key, val] : j.items()) {
        out << key << "," << val["size"] << "," << val["sha1"] << "\n";
    }
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

std::vector<Entry> scan_directory(const fs::path& path, uintmax_t& total_size, json& cache) {
    std::vector<Entry> entries;
    total_size = 0;

    for (const auto& entry : fs::directory_iterator(path)) {
        try {
            std::string name = entry.path().filename().string();
            uintmax_t size = 0;
            std::string type;
            std::string sha1;
            std::string full_path = entry.path().string();
            std::replace(full_path.begin(), full_path.end(), '\\', '/');

            std::cout << entry << " --> ";

            if (fs::is_directory(entry)) {
                type = "Folder";
                // size = 0;
                // for (auto& p : fs::recursive_directory_iterator(entry, fs::directory_options::skip_permission_denied)) {
                //     if (fs::is_regular_file(p)) {
                //         size += fs::file_size(p);
                //     }
                // }
                size = get_directory_size(entry);
                sha1 = "FOLDER";
            } else if (fs::is_regular_file(entry)) {
                size = fs::file_size(entry);
                type = "File";
                if (cache.contains(name) && cache[name]["size"] == size) {
                    sha1 = cache[name]["sha1"];
                } else {
                    sha1 = compute_sha1_file(entry);
                    cache[name] = { {"size", size}, {"sha1", sha1} };
                }
            } else continue;
            std::cout << sha1 << "\n";

            entries.push_back({name, type, size, sha1});
            total_size += size;
        } catch (...) { continue; }
    }
    return entries;
}

void print_table(const std::vector<Entry>& entries, uintmax_t total_size, int top_n = -1) {
    std::vector<Entry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const Entry& a, const Entry& b) {
        return a.size > b.size;
    });

    std::cout << std::left << std::setw(40) << "Name"
              << std::setw(10) << "Type"
              << std::setw(15) << "Size"
              << std::setw(12) << "% of Total" << "SHA1" << "\n";
    std::cout << std::string(110, '-') << "\n";

    int count = 0;
    for (const auto& e : sorted) {
        if (top_n != -1 && count >= top_n) break;
        double percent = total_size ? (e.size * 100.0 / total_size) : 0.0;
        std::cout << std::left << std::setw(40) << e.name
                  << std::setw(10) << e.type
                  << std::setw(15) << human_readable_size(e.size)
                  << std::setw(12) << std::fixed << std::setprecision(2) << percent << "% "
                  << e.sha1.substr(0, 10) << "..." << "\n";
        count++;
    }
    std::cout << "\nTotal Size: " << human_readable_size(total_size) << "\n";
}

void save_csv(const std::string& filename, const std::vector<Entry>& entries, uintmax_t total_size) {
    std::ofstream out(filename);
    std::vector<Entry> sorted = entries;
    std::sort(sorted.begin(), sorted.end(), [](const Entry& a, const Entry& b) {
        return a.size > b.size;
    });
    out << "SN,Name,Type,Size,Percent,SHA1\n";
    int c = 0;
    for (const auto& e : sorted) {
        double percent = total_size ? (e.size * 100.0 / total_size) : 0.0;
        out << c++ << ","
            << e.name << ","
            << e.type << ","
            << e.size << ","
            << std::fixed << std::setprecision(2) << percent << ","
            << e.sha1 << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <folder_path> [--top N] [--csv output.csv]\n";
        return 1;
    }
    auto start_time = std::chrono::high_resolution_clock::now();

    fs::path input_path(argv[1]);
    int top_n = -1;
    std::string csv_output;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--top" && i + 1 < argc) {
            top_n = std::stoi(argv[++i]);
        } else if (arg == "--csv" && i + 1 < argc) {
            csv_output = argv[++i];
        }
    }

    if (!fs::exists(input_path) || !fs::is_directory(input_path)) {
        std::cerr << "Invalid path.\n";
        return 1;
    }

    SetConsoleOutputCP(CP_UTF8);
    std::cout << "\n📁 Scanning: " << input_path << " ...\n\n";

    json cache = load_hashmap(input_path);
    uintmax_t total_size = 0;
    auto entries = scan_directory(input_path, total_size, cache);
    save_hashmap(input_path, cache);

    print_table(entries, total_size, top_n);
    if (!csv_output.empty()) {
        save_csv(csv_output, entries, total_size);
        std::cout << "\nCSV saved to: " << csv_output << "\n";
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;
    std::cout << "\n⏱️ Elapsed time: " << std::fixed << std::setprecision(2) << elapsed.count() << " seconds.\n";

    return 0;
}
