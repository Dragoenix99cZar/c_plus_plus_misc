#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include <unordered_map>
#include <bits/stdc++.h>

size_t longest_filename_length = 3;
int totalLinesOfCode = 0;
long totalsrcFileSize = 0;
// Extensions to scan
std::vector<std::string> extensions = { ".cpp", ".c", ".py", ".cs", ".h", ".hpp", ".ps1", ".html", ".js", ".css", ".json" };
std::unordered_set<std::string> notRecognizedFiles;

// Function to check if a file has the desired extension
bool hasExtension(const std::string& filePath, const std::vector<std::string>& extensions) 
{
	for (const auto& ext : extensions)
	{
		std::string nExt= filePath.substr(filePath.find_last_of('.'));
		std::cout << "Ext: " << filePath << " cmp: " << nExt.compare(ext) << "\n";
		if (filePath.size() >= ext.size() && filePath.compare(filePath.size() - ext.size(), ext.size(), ext) == 0)
		{
			if (filePath.size() > longest_filename_length)
			{
				longest_filename_length = filePath.size();
			}
			return true;
		}
	}
	return false;
}

// Function to count lines of code in a file
int countLinesOfCode(const std::string& filePath) {
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filePath << "\n";
		return 0;
	}
	int lineCount = 0;
	std::string line;
	while (std::getline(file, line)) {
		lineCount++;
	}
	file.close();
	totalLinesOfCode += lineCount;
	return lineCount;
}

// Function to get file size
long getFileSize(const std::string& filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.is_open()) {
		std::cerr << "Error opening file: " << filePath << "\n";
		return 0;
	}
	long size = file.tellg();
	file.close();
	totalsrcFileSize += size;
	return size;
}

// Function to traverse a directory and process source files
void traverseDirectory(const std::string& directoryPath, std::unordered_map<std::string, std::vector<std::string>>& fileData) {
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile((directoryPath + "\\*").c_str(), &findFileData);

	if (hFind == INVALID_HANDLE_VALUE) {
		std::cerr << "Error opening directory: " << directoryPath << "\n";
		return;
	}

	do {
		std::string fileName = findFileData.cFileName;
		if (fileName == "." || fileName == "..") continue;

		std::string fullPath = directoryPath + "\\" + fileName;

		if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// Recursively traverse subdirectories
			traverseDirectory(fullPath, fileData);
		}
		else if (hasExtension(fileName, extensions)) {
			// Process source files
			fileData[fileName] = { fullPath, std::to_string(countLinesOfCode(fullPath)), std::to_string(getFileSize(fullPath)) };
		}
	} while (FindNextFile(hFind, &findFileData) != 0);

	FindClose(hFind);
}

int main(int argc, char* argv[]) {
	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <directory_path>\n";
		std::cerr << "   eg: " << argv[0] << " . // current dir\n";
		std::cerr << "       " << argv[0] << " . <ext1> <ext2> ...\n";
		return EXIT_FAILURE;
	}
	if (argc > 2)
	{
		for (int i = 2; i < argc; i++)
		{
			extensions.push_back(argv[i]);
		}
	}
	std::cout << "\n\nLooking for following files:\n";
	for (auto f : extensions)
	{
		std::cout << f << ", ";
	}
	std::cout << "\n\n";

	std::string directoryPath = argv[1];
	std::unordered_map<std::string, std::vector<std::string>> fileData;

	// Traverse the directory
	traverseDirectory(directoryPath, fileData);

	// Display results
	std::cout << "Source Files in Directory: " << directoryPath << "\n";
	// std::cout << "------------------------------------------------------------\n";
	for (int i = 0; i < 100; i++) { std::cout << char(205); }
	std::cout << "\n";
	std::cout << std::setw(longest_filename_length) << std::setfill(' ')
		<< "Filename"
		<< std::setw(20) << std::setfill(' ')
		<< "Lines of Code"
		<< std::setw(20) << std::setfill(' ')
		<< "File Size (Bytes)" << "\n";
	for (int i = 0; i < 100; i++) { std::cout << char(205); }
	std::cout << "\n";
	// std::cout << "Filename        Lines of Code    File Size (Bytes)\n";
	// std::cout << "------------------------------------------------------------\n";
	int noOfFiles = 0;
	for (const auto& entry : fileData) {
		std::cout << std::setw(longest_filename_length) << std::setfill(' ')
			<< entry.first
			<< std::setw(15) << std::setfill(' ')
			<< entry.second[1]
			<< std::setw(15) << std::setfill(' ')
			<< entry.second[2] << "\n";

		noOfFiles++;
	}
	// std::cout << "longest_filename_length  : " << longest_filename_length << "\n";
	for (int i = 0; i < 100; i++) { std::cout << char(205); }
	std::cout << "\n";
	std::cout << "Total files              : " << noOfFiles << "\n";
	std::cout << "Total Lines of Code      : " << totalLinesOfCode << "\n";
	std::cout << "Total source files size  : " << (totalsrcFileSize / 1024) << "KB \n";
	for (int i = 0; i < 100; i++) { std::cout << char(205); }
	std::cout << "\n\n";
	std::cout << "Not Recognized files: ";
	for (auto f : notRecognizedFiles)
	{
		std::cout << f << ", ";
	}
	std::cout << "\n";


	return EXIT_SUCCESS;
}
