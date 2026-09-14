#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <algorithm>
#include <random> // Required for random number generation
#include <chrono> // Used to seed the generator

// ANSI color macros
#define GREEN   "\033[1;32m"
#define CYAN    "\033[1;36m"
#define YELLOW  "\033[1;33m"
#define WHITE   "\033[0;37m"
#define BWHITE  "\033[1;37m"
#define RESET   "\033[0m"

// Define a structure to hold the coordinates and pathfinding metrics
struct Node {
    int x, y;
    double gCost = std::numeric_limits<double>::infinity(); 
    double hCost = 0.0;                                    
    double fCost() const { return gCost + hCost; }          
    Node* parent = nullptr; // Note: We use coordinate mapping for robustness in A*

    // Min-heap comparison: lower F cost has higher priority
    bool operator<(const Node& other) const {
        return fCost() > other.fCost(); 
    }

    void printNode(){
        std::cout << "(" << x << ", " << y << ") -> ";
    }
};

using Grid = std::vector<std::vector<bool>>; // true = obstacle (wall), false = open path

// --- NEW FUNCTION: Random Grid Generation ---
/**
 * @brief Creates a grid where cells are obstacles based on the given density.
 * @param width The width of the grid (columns).
 * @param height The height of the grid (rows).
 * @param density The probability (0.0 to 1.0) that a cell is an obstacle.
 * @return The generated Grid.
 */
Grid generateRandomGrid(int width, int height, double density) {
    if (density < 0.0 || density > 1.0) {
        throw std::invalid_argument("Density must be between 0.0 and 1.0");
    }

    // Use a high-quality random number generator seeded by time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    // unsigned seed = 13;
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(0.0, 1.0);

    Grid map(height, std::vector<bool>(width));

    for (int y = 0; y < height; ++y) { // Iterate over rows (Y axis)
        for (int x = 0; x < width; ++x) { // Iterate over columns (X axis)
            double randomValue = dis(gen);
            if (randomValue < density) {
                // If the random value is less than the density, it becomes an obstacle
                map[y][x] = true; 
            } else {
                map[y][x] = false; // Open path
            }
        }
    }
    return map;
}
// ---------------------------------------------

// --- UTILITY FUNCTION TO PRINT THE GRID (unchanged) ---
void printGrid(const Grid& grid) {
    if (grid.empty()) return;

    int rows = grid.size();
    int cols = grid[0].size();

    std::cout << "\n=========================\n";
    for (int y = 0; y < rows; ++y) { // Iterate over rows (Y axis)
        for (int x = 0; x < cols; ++x) { // Iterate over columns (X axis)
            if (grid[y][x]) {
                std::cout << " # "; // Obstacle/Wall
            } else {
                std::cout << " . "; // Walkable path
            }
        }
        std::cout << "\n"; 
    }
    std::cout << "=========================\n\n";
}
// --- UTILITY FUNCTION TO PRINT THE GRID (changed) ---
bool contains(const std::vector<std::pair<int, int>>& vec, const int& x, const int& y) {
    for (int i = 0; i < vec.size(); ++i) {
        if (vec[i].first == x && vec[i].second == y ) {
            return true;
        }
    }
    return false;
}

void printGridComp(const Grid& grid, const std::vector<std::pair<int, int>>& path) {
    if (grid.empty()) return;

    int rows = grid.size();
    int cols = grid[0].size();

    std::cout << "\n=========================\n";
    std::cout << BWHITE;
    for (int y = 0; y < rows; ++y) { // Iterate over rows (Y axis)
        for (int x = 0; x < cols; ++x) { // Iterate over columns (X axis)
            if (grid[y][x]) {
                std::cout << " # "; // Obstacle/Wall
            } else {
                std::cout << ((contains(path, x, y))? GREEN : BWHITE);
                std::cout << ((contains(path, x, y))? " o " : " . ");
                // std::cout << " . "; // Walkable path
            }
            std::cout << BWHITE;
        }
        std::cout << "\n"; 
    }
    std::cout << "=========================\n\n";
}


// --- A* CORE LOGIC (unchanged, but adjusted for map-based parent tracking) ---

double calculateHeuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<std::pair<int, int>> reconstructPath(const std::map<std::pair<int, int>, std::pair<int, int>>& parentsMap, 
                                               std::pair<int, int> endCoord, 
                                               std::pair<int, int> startCoord) {
    std::vector<std::pair<int, int>> path;
    std::pair<int, int> currCoord = endCoord;

    while (currCoord != startCoord) {
        path.push_back(currCoord);
        // Look up the parent in the map
        auto it = parentsMap.find(currCoord);
        if (it == parentsMap.end()) break; // Safety break if path is corrupted

        currCoord = it->second; 
    }
    path.push_back(startCoord); // Add the starting point
    std::reverse(path.begin(), path.end()); 
    return path;
}


// NOTE: We pass `parents` map by reference so A* can modify it
std::vector<std::pair<int, int>> aStarSearch(const Grid& grid, 
                                               int startX, int startY, 
                                               int endX, int endY) {

    if (grid[startY][startX] || grid[endY][endX]) return {}; 
    
    int rows = grid.size();
    int cols = grid[0].size();
    std::pair<int, int> startCoord = {startX, startY};
    std::pair<int, int> endCoord = {endX, endY};

    std::priority_queue<Node> openSet; 
    // Map to track the best known G cost for every coordinate pair.
    std::map<std::pair<int, int>, double> gCosts;
    // Parent mapping: Coordinate -> Parent Coordinate
    std::map<std::pair<int, int>, std::pair<int, int>> parents;


    Node startNode = {startX, startY};
    startNode.hCost = calculateHeuristic(startX, startY, endX, endY);
    openSet.push(startNode);

    gCosts[startCoord] = 0.0;


    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        current.printNode();

        int currentX = current.x;
        int currentY = current.y;
        std::pair<int, int> currentCoord = {currentX, currentY};


        if (currentX == endX && currentY == endY) {
            // Path found! Reconstruct using the parent map.
            return reconstructPath(parents, endCoord, startCoord);
        }
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            int neighborX = currentX + dx[i];
            int neighborY = currentY + dy[i];

            // Bounds and Obstacle Check
            if (neighborX < 0 || neighborX >= cols || neighborY < 0 || neighborY >= rows || grid[neighborY][neighborX]) continue;

            double movementCost = 1.0;
            std::pair<int, int> neighborCoord = {neighborX, neighborY};
            double newGCost = gCosts[currentCoord] + movementCost;


            // Check if this path is better than any previously found path
            if (gCosts.find(neighborCoord) == gCosts.end() || newGCost < gCosts[neighborCoord]) {
                
                // Update state
                double h = calculateHeuristic(neighborX, neighborY, endX, endY);
                gCosts[neighborCoord] = newGCost;
                parents[neighborCoord] = currentCoord; // Record parent!

                Node neighbor = {neighborX, neighborY};
                neighbor.hCost = h;
                neighbor.gCost = newGCost;
                openSet.push(neighbor); 
            }
        }
    }

    return {}; // Path not found after exhausting all possibilities
}


// --- Utility function for printing the path coordinates (unchanged) ---
void printPath(const std::vector<std::pair<int, int>>& path) {
    if (path.empty()) {
        std::cout << "--- Result: NO PATH FOUND ---\n";
        return;
    }

    std::cout << "\n=========================\n";
    std::cout << "--- Path Found! Length: " << path.size() - 1 << " steps ---\n";
    for (const auto& p : path) {
        std::cout << "(" << p.first << ", " << p.second << ") -> ";
    }
    std::cout << "END\n";
    std::cout << "=========================\n";
}


// ==========================================
int main() {
    const int WIDTH = 20;
    const int HEIGHT = 15;
    double OBSTACLE_DENSITY = 0.05; // Try 0.7 to see a complex maze!

    try {
        // Generate the map randomly based on density
        Grid randomMap = generateRandomGrid(WIDTH, HEIGHT, OBSTACLE_DENSITY);

        // Display the generated map
        std::cout << "--- Generated Grid (Density: " << OBSTACLE_DENSITY * 100 << "%) ---\n";
        printGrid(randomMap);
        
        // --- Define Coordinates (X, Y) ---
        int startX = 0, startY = 0; // Top-left corner
        // int endX = WIDTH - 1, endY = HEIGHT - 1;   // Bottom-right corner
        int endX = 10, endY = 10;   // Bottom-right corner
        
        std::cout << "--- A* Search Started ---\n";
        std::cout << "Start: (" << startX << ", " << startY << ") | End: (" << endX << ", " << endY << ")\n\n";
        
        
        std::vector<std::pair<int, int>> shortestPath = aStarSearch(randomMap, startX, startY, endX, endY);
        
        printPath(shortestPath);
        printGridComp(randomMap, shortestPath);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }


    return 0;
}
