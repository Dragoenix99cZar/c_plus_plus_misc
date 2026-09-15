#!/usr/bin/env python3
import subprocess
import sys

COMMANDS = [
    "g++ exp_eval.cpp -o expEval",                                                              # 0
    "clang++ -std=c++17 -Wall -Wextra -ggdb -o benchmark_io.exe benchmark_io.cpp",              # 1
    "clang++ -std=c++17 -Wall -Wextra -ggdb codebaseAnalyzer.cpp -o codebaseAnalyzer.exe",      # 2
    "g++ -o percep perceptron.cpp",                                                             # 3
    "g++ -o mytool mytooldoc.cpp",                                                              # 4
    "clang++ -Wall -Wextra -ggdb levenshtein.cpp -o levenshtein"                                # 5
]

def run_cmd(cmd):
    print(f"\n[RUN] {cmd}")
    result = subprocess.run(cmd, shell=True, check=False)
    if result.returncode != 0:
        print(f"Error: Command failed with exit code {result.returncode}", file=sys.stderr)
        return False
    return True

def apply_compiler(cmd, new_cc):
    if not new_cc:
        return cmd
    parts = cmd.split(" ", 1)
    if len(parts) > 1:
        return f"{new_cc} {parts[1]}"
    return new_cc

def main():
    target_cc = None
    target_index = None

    # Simple argument parsing
    args = sys.argv[1:]
    i = 0
    while i < len(args):
        if args[i].startswith("--cc="):
            target_cc = args[i][5:].strip('"\'')
        elif args[i] == "--i" and i + 1 < len(args):
            try:
                target_index = int(args[i + 1])
            except ValueError:
                print(f"Error: Invalid index '{args[i + 1]}'", file=sys.stderr)
                sys.exit(1)
            i += 1
        i += 1

    if target_index is not None:
        if not (0 <= target_index < len(COMMANDS)):
            print(f"Error: Index {target_index} out of range (0 to {len(COMMANDS) - 1})", file=sys.stderr)
            sys.exit(1)

        cmd = apply_compiler(COMMANDS[target_index], target_cc)
        if not run_cmd(cmd):
            sys.exit(1)
    else:
        for idx, base_cmd in enumerate(COMMANDS):
            cmd = apply_compiler(base_cmd, target_cc)
            if not run_cmd(cmd):
                print("Build aborted due to errors.", file=sys.stderr)
                sys.exit(1)

    print("\nExecution completed successfully!")

if __name__ == "__main__":
    main()


# Usage Examples
# Save the script as builder.py and make it executable (optional on Linux/macOS):

# Bash
# chmod +x builder.py
# Run a specific command index with a custom compiler:

# Python
# ./builder.py --cc="gcc" --i 2
# Run all commands overriding the compiler with clang:

# Python
# ./builder.py --cc="clang"
