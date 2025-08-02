// g++ -o mytool mytooldoc.cpp
#include <bits/stdc++.h>
#include <string>

int main(int argc, char *argv[])
{
    // Decorative number patterns
    std::cout << " 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9 1 3 5 7 9\n";
    std::cout << "0 2 4 6 8 0 2 4 6 820 2 4 6 8 0 2 4 6 840 2 4 6 8 0 2 4 6 860 2 4 6 870\n";

    // Top border of the box
    std::cout << char(201); // Top-left corner
    for (int i = 0; i < 59; i++)
    {
        std::cout << char(205); // Horizontal line
    }
    std::cout << char(187) << "\n"; // Top-right corner

    // First box content (expEval)
    std::cout << char(186) // Vertical line
              << " expEval \"<math-expression>\"                               "
              << char(186) << "\n"; // Vertical line
    std::cout << char(186)          // Vertical line
              << "  e.g: expEval \"12.3 * 4.0 / 3.1 + 22.78\"                  "
              << char(186) << "\n"; // Vertical line

    // Middle border of the box
    std::cout << char(199); // Left intersection
    for (int i = 0; i < 59; i++)
    {
        std::cout << char(196); // Horizontal line
    }
    std::cout << char(182) << "\n"; // Right intersection

    // Second box content (argEval)
    std::cout << char(186) // Vertical line
              << " argEval \"<URL>\"                                           "
              << char(186) << "\n"; // Vertical line
    std::cout << char(186)          // Vertical line
              << "  e.g: argEval \"https://www.somedomain.com/file.extention\" "
              << char(186) << "\n"; // Vertical line

    // Middle border of the box
    std::cout << char(199); // Left intersection
    for (int i = 0; i < 59; i++)
    {
        std::cout << char(196); // Horizontal line
    }
    std::cout << char(182) << "\n"; // Right intersection

    // Third box content (argEval)
    std::cout << char(186) // Vertical line
              << " cpp_Analyzer <directory_path_or_file>                     "
              << char(186) << "\n"; // Vertical line
    std::cout << char(186)          // Vertical line
              << "  e.g: cpp_Analyzer C:\\path\\to\\source                      "
              << char(186) << "\n"; // Vertical line

    // Middle border of the box
    std::cout << char(199); // Left intersection
    for (int i = 0; i < 59; i++)
    {
        std::cout << char(196); // Horizontal line
    }
    std::cout << char(182) << "\n"; // Right intersection

    // Fourth box content (argEval)
    std::cout << char(186) // Vertical line
              << " codebaseStats <directory_path>                            "
              << char(186) << "\n"; // Vertical line
    std::cout << char(186)          // Vertical line
              << "  e.g: codebaseStats .                                     "
              << char(186) << "\n"
              << char(186)
              << "  e.g: codebaseStats C:\\path\\to\\source                     "
              << char(186) << "\n"; // Vertical line

    // Bottom border of the box
    std::cout << char(200); // Bottom-left corner
    for (int i = 0; i < 59; i++)
    {
        std::cout << char(205); // Horizontal line
    }
    std::cout << char(188) << "\n"; // Bottom-right corner

    // If an argument is provided, print ASCII characters from 128 to 254
    if (argc > 1)
    {
        int idx = 36;
        int col = 1;
        while(idx < 256)
        {
            std::cout << std::setw(3) << idx << ": " << char(idx) << " \t";
            if(col > 10 )
            {
                std::cout << "\n";
                col = 0;
            }
            col++;
            idx = (idx < 256)? idx + 1 : 255 ;

        }
    }

    return 0;
}
