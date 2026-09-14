#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <map>
#include <algorithm>
#include <limits> // Required for infinity

// Define a structure to hold the coordinates and pathfinding metrics
struct Node {
    int x, y;
    double gCost = std::numeric_limits<double>::infinity(); 
    double hCost = 0.0;                                    
    double fCost() const { return gCost + hCost; }          
    Node* parent = nullptr;

    // Min-heap comparison: lower F cost has higher priority (comes "less" than others)
    bool operator<(const Node& other) const {
        return fCost() > other.fCost(); 
    }
};

using Grid = std::vector<std::vector<bool>>; // true = obstacle (wall), false = open path

// --- NEW UTILITY FUNCTION TO PRINT THE GRID ---
void printGrid(const Grid& grid) {
    if (grid.empty()) return;

    int rows = grid.size();
    int cols = grid[0].size();

    std::cout << "\n=========================\n";
    for (int y = 0; y < rows; ++y) { // Iterate over rows (Y axis)
        for (int x = 0; x < cols; ++x) { // Iterate over columns (X axis)
            // Print '#' if the cell is an obstacle (true), otherwise print '.'
            if (grid[y][x]) {
                std::cout << " # "; // Obstacle/Wall
            } else {
                std::cout << " . "; // Walkable path
            }
        }
        std::cout << "\n"; // Newline after each row
    }
    std::cout << "=========================\n\n";
}
// -----------------------------------------------


/**
 * @brief Calculates the Manhattan distance heuristic (L1 norm).
 */
double calculateHeuristic(int x1, int y1, int x2, int y2) {
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

// ... [reconstructPath and aStarSearch remain the same] ...
std::vector<std::pair<int, int>> reconstructPath(Node* endNode) {
    std::vector<std::pair<int, int>> path;
    Node* current = endNode;
    while (current != nullptr) {
        path.push_back({current->x, current->y});
        current = current->parent; 
    }
    std::reverse(path.begin(), path.end()); 
    return path;
}

std::vector<std::pair<int, int>> aStarSearch(const Grid& grid, 
                                               int startX, int startY, 
                                               int endX, int endY) {

    // Basic checks (omitted for brevity but assumed functional)
    if (grid[startY][startX] || grid[endY][endX]) return {}; 
    
    int rows = grid.size();
    int cols = grid[0].size();

    std::priority_queue<Node> openSet; 
    // We need a map to track the best known path and the parent relationship
    // Storing Node pointers here makes memory management complex, so we will simulate parent tracking by coordinates.
    std::map<std::pair<int, int>, double> gCosts;
    // Parent mapping: Coordinate -> Parent Coordinate
    std::map<std::pair<int, int>, std::pair<int, int>> parents;


    Node startNode = {startX, startY};
    startNode.hCost = calculateHeuristic(startX, startY, endX, endY);
    openSet.push(startNode);

    gCosts[{startX, startY}] = 0.0;


    while (!openSet.empty()) {
        Node current = openSet.top();
        openSet.pop();

        int currentX = current.x;
        int currentY = current.y;

        if (currentX == endX && currentY == endY) {
            // Path found! Reconstruct using the parent map.
            std::vector<std::pair<int, int>> path;
            std::pair<int, int> currCoord = {endX, endY};
            while (currCoord != std::make_pair(startX, startY)) {
                path.push_back(currCoord);
                // Retrieve the parent coordinate from our map
                currCoord = parents[currCoord]; 
            }
            path.push_back({startX, startY}); // Add the starting point
            std::reverse(path.begin(), path.end());
            return path;
        }
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < 4; ++i) {
            int neighborX = currentX + dx[i];
            int neighborY = currentY + dy[i];

            // Bounds and Obstacle Check
            if (neighborX < 0 || neighborX >= cols || neighborY < 0 || neighborY >= rows || grid[neighborY][neighborX]) continue;

            double movementCost = 1.0;
            double newGCost = gCosts[{currentX, currentY}] + movementCost;
            std::pair<int, int> neighborCoord = {neighborX, neighborY};

            // Check if this path is better than any previously found path
            if (gCosts.find(neighborCoord) == gCosts.end() || newGCost < gCosts[neighborCoord]) {
                
                // Update state
                double h = calculateHeuristic(neighborX, neighborY, endX, endY);
                gCosts[neighborCoord] = newGCost;
                parents[neighborCoord] = {currentX, currentY}; // Record parent!

                Node neighbor = {neighborX, neighborY};
                neighbor.hCost = h;
                neighbor.gCost = newGCost;
                openSet.push(neighbor); 
            }
        }
    }

    return {}; // Path not found
}


// --- Utility function for printing the path coordinates (remains the same) ---
void printPath(const std::vector<std::pair<int, int>>& path) {
    if (path.empty()) {
        std::cout << "--- Result: NO PATH FOUND ---\n";
        return;
    }

    std::cout << "\n=========================\n";
    std::cout << "--- Path Found! Length: " << path.size() - 1 << " steps ---\n";
    // We could optionally print a visual path on the grid here, but for simplicity:
    for (const auto& p : path) {
        std::cout << "(" << p.first << ", " << p.second << ") -> ";
    }
    std::cout << "END\n";
    std::cout << "=========================\n";
}


// ==========================================
int main() {
    const int WIDTH = 30;
    const int HEIGHT = 30;

    Grid map(HEIGHT, std::vector<bool>(WIDTH, false));

    // --- Define Obstacles (Walls) ---
    for (int y = 2; y < 8; ++y) {
        map[y][5] = true; // Central wall column
    }
    map[1][1] = true;
    map[1][2] = true;

    // Display the map before running the search!
    std::cout << "--- Initial Grid State ---\n";
    printGrid(map);


    // --- Define Coordinates (X, Y) ---
    int startX = 14, startY = 18; // Top-left corner
    int endX = 0, endY = 9;   // Bottom-right corner

    std::cout << "--- A* Search Started ---\n";
    std::cout << "Start: (" << startX << ", " << startY << ") | End: (" << endX << ", " << endY << ")\n\n";


    std::vector<std::pair<int, int>> shortestPath = aStarSearch(map, startX, startY, endX, endY);
    
    printPath(shortestPath);

    return 0;
}
