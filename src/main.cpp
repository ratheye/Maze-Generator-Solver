#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm> 

using namespace std;

// Direction arrays (North, South, East, West)
const int DX[4] = {0, 0, 1, -1};
const int DY[4] = {-1, 1, 0, 0};

class Maze {
private:
    int width;
    int height;
    vector<vector<char>> grid;
    vector<vector<bool>> visited;

public:
    Maze(int w, int h) : width(w), height(h) {
        // Initialize grid with Walls ('#')
        grid.resize(height, vector<char>(width, '#'));
        visited.resize(height, vector<bool>(width, false));
        srand(time(0)); 
    }

    bool isValid(int x, int y) {
        return (x >= 0 && x < width && y >= 0 && y < height);
    }

    // Generate maze
    void generate(int x, int y) {
        visited[y][x] = true;
        grid[y][x] = ' '; // Carve path

        int dirs[4] = {0, 1, 2, 3};
        random_shuffle(begin(dirs), end(dirs));

        for (int i = 0; i < 4; ++i) {
            int nx = x + (DX[dirs[i]] * 2); 
            int ny = y + (DY[dirs[i]] * 2);

            if (isValid(nx, ny) && !visited[ny][nx]) {
                grid[y + DY[dirs[i]]][x + DX[dirs[i]]] = ' '; // Knock down wall
                generate(nx, ny);
            }
        }
    }

    // Solver: Recursive DFS
    bool solveDFS(int x, int y) {
        // Check if the exit has been found
        if (grid[y][x] == 'E') {
            return true;
        }

        // Invalid Move Checks: checks  if it is a Wall (#) or a Path we already went down (.)
        if (grid[y][x] == '#' || grid[y][x] == '.') {
            return false;
        }

        // Mark path
        if (grid[y][x] != 'S') {
            grid[y][x] = '.'; 
        }

        // 4. Try all 4 directions
        for (int i = 0; i < 4; ++i) {
            if (solveDFS(x + DX[i], y + DY[i])) {
                return true;
            }
        }

        //Backtrack: If dead end, unmark path unless it's the start
        if (grid[y][x] != 'S') {
            grid[y][x] = ' '; 
        }
        return false;
    }

    void display() {
        for (const auto& row : grid) {
            for (char cell : row) {
                cout << cell << " "; 
            }
            cout << endl;
        }
    }

    void run() {
        // Generate maze starting at (1,1)
        cout << "Generating Random Maze..." << endl;
        generate(1, 1);
        
        // Setup Start and Exit
        int exitX = width - 2;
        int exitY = height - 2;

        grid[1][1] = 'S';           // Mark Start
        grid[exitY][exitX] = 'E';   // Mark Exit 

        cout << "Maze Generated (S = Start, E = Exit):" << endl;
        display();

        cout << "\nSolving..." << endl;
        // Start solving from (1,1)
        if (solveDFS(1, 1)) {
            cout << "\nSOLVED! ('.' is the path)" << endl;
            display();
        } else {
            cout << "No solution found." << endl;
        }
    }
};

int main() {
    // Ensuring the dimensions are odd numbers
    Maze myMaze(21, 21);
    myMaze.run();
    return 0;
}
