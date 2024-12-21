#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <set>
#include <algorithm>

using namespace std;

struct Cell {
    int parent_i, parent_j;
    double f, g, h;

    Cell() : parent_i(0), parent_j(0), f(INFINITY), g(INFINITY), h(0) {}
};

int ROW = 0;
int COL = 0;

bool is_valid(int row, int col) {
    return (row >= 0) && (row < ROW) && (col >= 0) && (col < COL);
}

bool is_unblocked(const vector<vector<int>>& grid, int row, int col) {
    return grid[row][col] == 1;
}

bool is_end(int row, int col, const pair<int, int>& dest) {
    return row == dest.first && col == dest.second;
}

double h_point(int row, int col, const pair<int, int>& dest) {
    return sqrt(pow(row - dest.first, 2) + pow(col - dest.second, 2));
}

vector<pair<int, int>> trace(const vector<vector<Cell>>& cell_details, const pair<int, int>& dest) {
    vector<pair<int, int>> path;
    int row = dest.first;
    int col = dest.second;

    while (!(cell_details[row][col].parent_i == row && cell_details[row][col].parent_j == col)) {
        path.push_back({row, col});
        int temp_row = cell_details[row][col].parent_i;
        int temp_col = cell_details[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }

    path.push_back({row, col});
    reverse(path.begin(), path.end());
    return path;
}

vector<pair<int, int>> dfs(const vector<vector<int>>& grid, pair<int, int> start, const pair<int, int>& dest, set<pair<int, int>>& visited) {
    if (!is_valid(start.first, start.second) || !is_unblocked(grid, start.first, start.second)) {
        return {};
    }

    if (start == dest) {
        return {start};
    }

    visited.insert(start);

    vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    for (const auto& dir : directions) {
        int new_i = start.first + dir.first;
        int new_j = start.second + dir.second;

        if (is_valid(new_i, new_j) && is_unblocked(grid, new_i, new_j) && visited.find({new_i, new_j}) == visited.end()) {
            vector<pair<int, int>> path = dfs(grid, {new_i, new_j}, dest, visited);
            if (!path.empty()) {
                path.insert(path.begin(), start);
                return path;
            }
        }
    }

    return {};
}

bool a_star(const vector<vector<int>>& grid, const pair<int, int>& src, const pair<int, int>& dest, bool print_path = true) {
    if (!is_valid(src.first, src.second) || !is_valid(dest.first, dest.second)) {
        cout << "Không h?p l?~!!!" << endl;
        return false;
    }

    if (!is_unblocked(grid, src.first, src.second) || !is_unblocked(grid, dest.first, dest.second)) {
        cout << "Không th? di chuy?n~!!!" << endl;
        return false;
    }

    if (is_end(src.first, src.second, dest)) {
        cout << "Ðã t?i dích~!!!" << endl;
        return true;
    }

    vector<vector<bool>> closed_list(ROW, vector<bool>(COL, false));
    vector<vector<Cell>> cell_details(ROW, vector<Cell>(COL));

    int i = src.first;
    int j = src.second;
    cell_details[i][j].f = 0;
    cell_details[i][j].g = 0;
    cell_details[i][j].h = 0;
    cell_details[i][j].parent_i = i;
    cell_details[i][j].parent_j = j;

    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> open_list;
    open_list.push({0.0, {i, j}});

    bool found_dest = false;

    while (!open_list.empty()) {
        pair<double, pair<int, int>> p = open_list.top();
        open_list.pop();

        i = p.second.first;
        j = p.second.second;
        closed_list[i][j] = true;

        vector<pair<int, int>> directions = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (const auto& dir : directions) {
            int new_i = i + dir.first;
            int new_j = j + dir.second;

            if (is_valid(new_i, new_j) && is_unblocked(grid, new_i, new_j) && !closed_list[new_i][new_j]) {
                if (is_end(new_i, new_j, dest)) {
                    cell_details[new_i][new_j].parent_i = i;
                    cell_details[new_i][new_j].parent_j = j;
                    if (print_path) {
                        vector<pair<int, int>> path = trace(cell_details, dest);
                        cout << "-> ";
                        for (const auto& point : path) {

