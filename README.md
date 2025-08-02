# DSA C++ Tools Collection

A comprehensive collection of C++ utilities, algorithms, and benchmarking tools for data structures, algorithms, and system programming tasks.

## 📁 Project Structure

```
cpp/
├── .vscode/           # VS Code configuration files
├── mytool/           # Compiled executables and tools
├── outp/             # Output files and compiled binaries
├── *.cpp             # Source files (main implementations)
├── *.hpp             # Header files
├── *.txt             # Data files and outputs
└── *.exe             # Compiled executables
```

## 🛠️ Complete Tools & Utilities Reference

### **Analysis & Benchmarking Tools**

#### 1. **Codebase Analyzer** (`codebaseAnalyzer.cpp`)
Powerful directory scanner that analyzes source code files and provides detailed statistics.

**Features:**
- Recursive directory traversal
- Multi-language support (C++, C, Python, C#, JavaScript, Java, PowerShell)
- Line counting and file size analysis
- Colored terminal output with emoji support
- Customizable file extension filtering
- Performance timing

**Usage:**
```bash
clang++ -std=c++17 -Wall -Wextra -ggdb codebaseAnalyzer.cpp -o codebaseStats
./codebaseStats <directory_path> [extensions...] [--exclude ext1 ext2...]
./codebaseStats . .cpp .h --exclude .txt .md
```

#### 2. **Legacy Code Analyzer** (`codeAnalyzer.cpp`, `old_codebaseAnalyzer.cpp`)
Earlier versions of the codebase analyzer with different feature sets.

**Usage:**
```bash
g++ codeAnalyzer.cpp -o codeAnalyzer
./codeAnalyzer <directory_path>
```

#### 3. **I/O Benchmark Tool** (`benchmark_io.cpp`)
Comprehensive file I/O performance benchmarking utility comparing different methods.

**Benchmarks:**
- File size detection (fseek vs ifstream vs filesystem)
- Line counting (getline vs iterator)
- Performance timing in microseconds
- Colored output with detailed statistics

**Usage:**
```bash
clang++ -std=c++17 -Wall -Wextra -ggdb -o benchmark_io benchmark_io.cpp
./benchmark_io <directory_path>
```

#### 4. **Directory Tree Scanner** (`wirTree_cached.cpp`, `wirTree_uncached.cpp`)
Advanced directory analysis tools with SHA1 hashing and caching capabilities.

**Features (Cached Version):**
- SHA1 hash computation for files
- JSON-based caching system
- Human-readable size formatting
- CSV export functionality
- Top-N filtering

**Usage:**
```bash
g++ -std=c++17 wirTree_cached.cpp -o dirTree
./dirTree <folder_path> [--top N] [--csv output.csv]
./dirTree /path/to/scan --top 10 --csv results.csv
```

**Features (Uncached Version):**
- Simple directory scanning without caching
- Faster for one-time scans

**Usage:**
```bash
g++ wirTree_uncached.cpp -o dirScan
./dirScan <folder_path>
```

### **Mathematical & Algorithm Tools**

#### 5. **Expression Evaluator** (`exp_eval.cpp`)
Mathematical expression parser and evaluator supporting infix notation.

**Features:**
- Supports basic arithmetic operators (+, -, *, /, ^)
- Parentheses handling
- Floating-point arithmetic
- Command-line expression evaluation
- Built-in test suite

**Usage:**
```bash
g++ exp_eval.cpp -o expEval
./expEval "2 + 3 * 4" "(5 + 3) * 2.5" "3^2"
```

#### 6. **New Expression Evaluator** (`new_exp_eval.cpp`)
Improved version of the expression evaluator with enhanced features.

**Usage:**
```bash
g++ new_exp_eval.cpp -o newExpEval
./newExpEval "complex_expression_here"
```

#### 7. **Levenshtein Distance Calculator** (`levenshtein.cpp`)
Edit distance computation with visualization capabilities.

**Features:**
- Dynamic programming implementation
- DP table visualization
- Operation tracking (insert, delete, substitute)
- Performance timing

**Usage:**
```bash
clang++ -O2 levenshtein.cpp -o diffw
./diffw string1 string2
```

#### 8. **Bit Operations Demo** (`bit_op.cpp`)
Demonstrates various bit manipulation techniques and operations.

**Features:**
- Binary/hex/decimal conversion
- Bit masking demonstrations
- Character encoding examples
- Educational bit manipulation examples

**Usage:**
```bash
g++ bit_op.cpp -o bitDemo
./bitDemo
```

### **Text Processing & Filtering Tools**

#### 9. **Word Filter Tool** (`filter_word.cpp`)
Efficient word filtering utility for finding words with unique characters.

**Features:**
- Filters words by length (configurable minimum)
- Identifies words with all unique characters
- Bit manipulation optimization
- Performance timing
- Unicode/emoji support

**Usage:**
```bash
clang++ -Wall -Wextra -ggdb filter_word.cpp -o filter_word
./filter_word words_alpha.txt
```

### **System & Utility Tools**

#### 10. **Argument Parser Test** (`arg_parse_test.cpp`)
No-auth curl wrapper with filename extraction capabilities.

**Features:**
- URL argument parsing
- Automatic filename extraction from URLs
- Curl command construction and execution
- Debugging output

**Usage:**
```bash
g++ arg_parse_test.cpp -o argEval
./argEval <URL>
./argEval https://example.com/file.txt
```

#### 11. **Custom Tee Implementation** (`mytee.c`)
Custom implementation of the Unix `tee` command.

**Usage:**
```bash
gcc mytee.c -o tee
echo "Hello World" | ./tee output.txt
```

#### 12. **File Duplication Tool** (`twice.c`)
Utility for file operations and duplication.

**Usage:**
```bash
gcc twice.c -o twice
./twice input_file output_file
```

#### 13. **Read Line Utility** (`rdline.c`)
Simple line reading utility for text processing.

**Usage:**
```bash
gcc rdline.c -o rdline
./rdline < input.txt
```

### **Game & Algorithm Implementations**

#### 14. **High-Performance Game Engine** (`game.cpp`)
Optimized game state management system with pathfinding algorithms.

**Features:**
- Bit manipulation for game state representation
- BFS pathfinding implementation
- Performance optimization with GCC pragmas
- 128-bit integer support for large game boards
- Benchmark testing capabilities

**Usage:**
```bash
g++ -O3 -march=native game.cpp -o game
./game
```

#### 15. **FizzBuzz Variants** (`fizzbaz.c`)
Demonstration of FizzBuzz algorithm and Fibonacci sequence.

**Features:**
- Classic FizzBuzz implementation
- Fibonacci sequence generator
- Educational programming examples

**Usage:**
```bash
gcc fizzbaz.c -o fizzbaz
./fizzbaz
```

### **Machine Learning & AI Tools**

#### 16. **Perceptron Implementation** (`perceptron.cpp`)
Basic neural network perceptron with random weight initialization.

**Features:**
- Random weight generation
- Configurable random seed
- Basic perceptron structure

**Usage:**
```bash
g++ -o percep perceptron.cpp
./percep
```

### **Cryptography & Security Tools**

#### 17. **SHA1 Hash Tester** (`shaTest.cpp`)
Testing utility for SHA1 hash implementation using TinySHA1.hpp.

**Features:**
- SHA1 hash computation
- Hash verification
- File hashing capabilities

**Usage:**
```bash
g++ shaTest.cpp -o shaTest
./shaTest input_file
```

### **Documentation Tools**

#### 18. **Tool Documentation Generator** (`mytooldoc.cpp`)
Utility for generating documentation for the tool collection.

**Usage:**
```bash
g++ mytooldoc.cpp -o docGen
./docGen
```

## 📚 Additional Components

### Hash Utilities
- **TinySHA1.hpp**: Lightweight SHA-1 hashing implementation
- **shaTest.cpp**: SHA-1 testing utility

### System Tools
- **mytee.c**: Custom `tee` command implementation
- **rdline.c**: Line reading utility
- **twice.c**: File duplication utility

### Data Files
- **words_alpha.txt**: Large English word dictionary (4.2MB)
- **filtered_words.txt**: Processed word list output
- **hashmap.txt**: Hash map data structure documentation
- **scanout.csv**: Analysis output data

## 🔧 Build Instructions

### Prerequisites
- GCC/Clang compiler with C++17 support
- Windows (some utilities use Windows-specific APIs)
- Make sure to enable ANSI color support for colored output

### Compilation Examples
```bash
# Codebase Analyzer
clang++ -std=c++17 -Wall -Wextra -ggdb codebaseAnalyzer.cpp -o codebaseStats

# Benchmark Tool
clang++ -std=c++17 -Wall -Wextra -ggdb -o benchmark_io benchmark_io.cpp

# Expression Evaluator
g++ exp_eval.cpp -o expEval

# Word Filter
clang++ -Wall -Wextra -ggdb filter_word.cpp -o filter_word

# High-Performance Game
g++ -O3 -march=native game.cpp -o game

# Levenshtein Distance
clang++ -O2 levenshtein.cpp -o diffw
```

## 🚀 Performance Features

- **SIMD Optimizations**: Game engine uses AVX2 and SSE instructions
- **Bit Manipulation**: Efficient algorithms using bitwise operations
- **Memory Optimization**: 128-bit integers for large data structures
- **Compiler Optimizations**: GCC pragmas for maximum performance
- **Benchmarking**: Built-in timing for performance analysis

## 📊 Output Features

- **Colored Terminal Output**: ANSI color codes for better readability
- **Unicode Support**: Emoji and special characters
- **Formatted Tables**: Aligned output for data presentation
- **Progress Indicators**: Real-time feedback during processing
- **Statistical Analysis**: Comprehensive metrics and averages

## 🎯 Use Cases

1. **Code Analysis**: Analyze large codebases for metrics and statistics
2. **Performance Testing**: Benchmark different I/O methods and algorithms
3. **Mathematical Computing**: Evaluate complex mathematical expressions
4. **Text Processing**: Filter and analyze large text datasets
5. **Game Development**: High-performance game state management
6. **String Algorithms**: Edit distance and string similarity calculations
7. **Machine Learning**: Basic neural network implementations

## 📝 Notes

- Most tools support command-line arguments for customization
- Windows-specific features require Windows headers
- Some tools generate output files (filtered_words.txt, scanout.csv)
- Executables are compiled to the `mytool/` directory
- Performance-critical code uses compiler-specific optimizations

## 🤝 Contributing

This is a collection of educational and utility tools. Each component is self-contained and can be modified independently. The codebase demonstrates various C++ programming techniques, from basic file I/O to advanced bit manipulation and SIMD optimizations.

## 📄 License

This project is licensed under the MIT License.

MIT License

Copyright (c) 2025 DSA C++ Tools Collection

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

The MIT License allows you to:
- ✅ Use the code commercially
- ✅ Modify and distribute the code
- ✅ Include in private projects
- ✅ Sublicense the code

Requirements:
- 📝 Include the original copyright notice
- 📝 Include the license text in distributions
