// g++ arg_parse_test.cpp -o argEval
#include <bits/stdc++.h>
#include <string>

#define string std::string

// Function to extract the filename from a given path
string getFilename(const string& src) {
    size_t lastSlash = src.rfind('/'); // Find the last occurrence of '/'
    if (lastSlash == string::npos) {
        // If no '/' is found, return the entire string as the filename
        return src;
    }
    // Extract the substring after the last '/'
    return src.substr(lastSlash + 1);
}

int main(int argc, char* argv[]) {
    // Check if the required URL argument is provided
    if (argc < 2) {
        std::cout << "argEval is a no-auth curl wrapper.\n"
                  << "Usage: argEval <URL>\n";
        return 1;
    }

    // Print the first two arguments for debugging
    for (int i = 0; i < 2; ++i) {
        std::cout << "Argument " << i << ": " << argv[i] << "\n";
    }

    // Construct the curl command
    string cmd = "curl ";
    cmd.append(argv[1]); // Append the URL
    cmd.append(" -o ");  // Append the output flag
    cmd.append(getFilename(argv[1])); // Append the filename

    // Print the constructed command
    std::cout << "Command:\n" << cmd << "\n";
    std::cout << "--------------------\n";

    // Execute the curl command
    system(cmd.c_str());

    return 0;
}
