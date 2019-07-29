//
//  main.cpp
//  Intro Astar search
//
//  Created by Gabriel Dalessandro on 7/25/19.
//  Copyright © 2019 Gabriel Dalessandro. All rights reserved.
//

#include <algorithm>  // for sort
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::sort;
using std::string;
using std::vector;
using std::abs;

// TODO: Add kStart and kFinish enumerators to the State enum.
enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
        if (n == 0) {
            row.push_back(State::kEmpty);
        } else {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
    ifstream myfile (path);
    vector<vector<State>> board{};
    if (myfile) {
        string line;
        while (getline(myfile, line)) {
            vector<State> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}


/**
 * Compare the F values of two cells.
 */
bool Compare(const vector<int> a, const vector<int> b) {
    int f1 = a[2] + a[3]; // f1 = g1 + h1
    int f2 = b[2] + b[3]; // f2 = g2 + h2
    return f1 > f2;
}


/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v) {
    sort(v->begin(), v->end(), Compare);
}


// Calculate the manhattan distance
int Heuristic(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
}


/**
 * Check that a cell is valid: on the grid, not an obstacle, and clear.
 */
bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());
    if (on_grid_x && on_grid_y)
        return grid[x][y] == State::kEmpty;
    return false;
}


/**
 * Add a node to the open list and mark it as open.
 */
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Add node to open vector, and mark grid cell as closed.
    openlist.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}


/**
 * Expand current nodes's neighbors and add them to the open list.
 */
void ExpandNeighbors(const vector<int> &current, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
    // Get current node's data.
    int x = current[0];
    int y = current[1];
    int g = current[2];
    
    // Loop through current node's potential neighbors.
    for (int i = 0; i < 4; i++) {
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];
        
        // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
        if (CheckValidCell(x2, y2, grid)) {
            // Increment g value and add neighbor to open list.
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, openlist, grid);
        }
    }
}


/**
 * Implementation of A* search algorithm
 */
vector<vector<State>> Search(vector<vector<State>> grid, int init[2], int goal[2]) {
    // Create the vector of open nodes.
    vector<vector<int>> open {};
    
    // Initialize the starting node.
    int x = init[0];
    int y = init[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0],goal[1]);
    AddToOpen(x, y, g, h, open, grid);
    
    while (open.size() > 0) {
        // Get the next node
        CellSort(&open);
        auto current = open.back();
        open.pop_back();
        x = current[0];
        y = current[1];
        grid[x][y] = State::kPath;
        
        // Check if we're done.
        if (x == goal[0] && y == goal[1]) {
            // TODO: Set the init grid cell to kStart, and
            // set the goal grid cell to kFinish before returning the grid.
            grid[init[0]][init[1]] = State::kStart;
            grid[goal[0]][goal[1]] = State::kFinish;
            return grid;
        }
        
        // If we're not done, expand search to current node's neighbors.
        ExpandNeighbors(current, goal, open, grid);
    }
    
    // We've run out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return std::vector<vector<State>>{};
}


string CellString(State cell) {
    switch(cell) {
        case State::kObstacle: return "⛰️   ";
        case State::kPath: return "🚗  ";
        case State::kStart: return "🚦  ";
        case State::kFinish: return "🏁   ";
        default: return "◼️   ";
    }
}


void PrintBoard(const vector<vector<State>> board) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            cout << CellString(board[i][j]);
        }
        cout << "\n";
    }
}

void SelectingStartEnd () {
    
}

#include "test.cpp"

int main() {
    auto board = ReadBoardFile("/Users/gabe/Desktop/Intro Astar search - Code/board.txt");
    std::cout << std::endl;
    PrintBoard(board);
    std::cout << std::endl;
    //declare the start, end and size of map
    int start[2];
    int goal[2];
    
    //    int start[2]{0, 0};
    //    int goal[2]{5, 6};
    
    int xInput;
    int yInput;
    
    string userInput;
    string inputXstr;
    string inputYstr;
    
    bool validCoordinates = false;
    while (!validCoordinates) {
        cout << "Enter the starting position (x, y): ";
        std::getline(std::cin, userInput);
        
        inputXstr = userInput[1];
        inputYstr = userInput[4];
        
        std::stringstream ssX(inputXstr);
        ssX >> xInput;
        std::stringstream ssY(inputYstr);
        ssY >> yInput;
        
        start[0] = xInput;
        start[1] = yInput;
        //cout << endl << start[0] << start[1] << endl;
        
        if ( CheckValidCell(xInput, yInput, board) ) {
            validCoordinates = true;
        }
        else {
            std::cout << "Invalid coordinates..." << std::endl;
        }
    }
    
    validCoordinates = false;
    while (!validCoordinates) {
        cout << "Enter the ending position (x, y): ";
        std::getline(std::cin, userInput);
        
        inputXstr = userInput[1];
        inputYstr = userInput[4];
        
        std::stringstream ssX2(inputXstr);
        ssX2 >> xInput;
        std::stringstream ssY2(inputYstr);
        ssY2 >> yInput;
        
        goal[0] = xInput;
        goal[1] = yInput;
        //cout << endl << goal[0] << goal[1] << endl;
        
        if ( CheckValidCell(xInput, yInput, board) ) {
            validCoordinates = true;
        }
        else {
            std::cout << "Invalid coordinates..." << std::endl;
        }
    }
    
    

    
    auto solution = Search(board, start, goal);
    PrintBoard(solution);
    
    //    TestHeuristic();
    //    TestAddToOpen();
    //    TestCompare();
    //    TestSearch();
    //    TestCheckValidCell();
    //    TestExpandNeighbors();
    return 0;
}
