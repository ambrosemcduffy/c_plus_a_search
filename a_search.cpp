#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using std::vector;
using std::cout;
using std::ifstream;
using std::string;
using std::istringstream;
using std::abs;
using std::sort;

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};


vector<State> ParseLine(string word){
    /*
     * Parsing the strings, and converting digits to ints, and commas
     * as commas value.
     */


    // Initializing out variables
    int num;
    char comma;

    // Initialzing our vector to take in States
    vector<State> data;

    //Initializing a stream
    istringstream mystream(word);

    // pushing states into array
    while (mystream >> num >> comma && comma == ','){
        if (num == 0){
            data.push_back(State::kEmpty);
        }
        else{
            data.push_back(State::kObstacle);
        }
    }
    return data;
}


vector<vector<State>> readBoardFile(){
    /*
     * Reading in board file, and returning the arrays as 
     * strings.
     */
    
    ifstream myfile;
    myfile.open("1.board");
    vector<vector<State>> board = {};
    
    if (myfile){
        string line;
        while (getline(myfile, line)){
            board.push_back(ParseLine(line));        
        }
    }
    return board;
}


bool Compare(const vector<int> node1, const vector<int> node2){
    int f1 = node1[2] + node1[3];
    int f2 = node2[2] + node2[3];
    return f1 > f2;
}


void CellSort(vector<vector<int>> *v){
    sort(v->begin(), v->end(), Compare);
}


int Heuristic(int x1, int x2, int y1, int y2){
    /*
     Manhattan Distance Heuristics function
     */
    return abs(x2-x1) + abs(y2-y1);
}


bool CheckValidCell(int x, int y, vector<vector<State>> &board){
    bool on_board_x = (x >= 0 && x < board.size());
    bool on_board_y = (y >= 0 && y < board[0].size());

    if (on_board_x && on_board_y)
        return board[x][y] == State::kEmpty;
    return false;
}


void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open_node, vector<vector<State>> &board){
    vector<int> node = {x, y, g, h};
    open_node.push_back(node);
    board[x][y] = State::kClosed;
}


void ExpandNeighbors(const vector<int> &current, vector<vector<int>> &open, vector<vector<State>> &board, int goal[2]){
    int x = current[0];
    int y = current[1];
    int g = current[2];

    for (int i=0; i < 4; i ++){
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        if (CheckValidCell(x2, y2, board)){
            int g2 = g+1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, open, board);
        }
    }
}


vector<vector<State>> Search(vector<vector<State>> board, int start[2], int goal[2]){
    vector<vector<int>> open = {};
    
    int x = start[0];
    int y = start[1];
    int g = 0;
    int h = Heuristic(x, y, goal[0], goal[1]);
    
    AddToOpen(x, y, g, h, open, board);

    while (open.size() > 0){
        CellSort(&open);
        auto current = open.back();
        open.pop_back();

        x = current[0];
        y = current[1];
        board[x][y] = State::kPath;
        
        if (x == goal[0] && y == goal[1]){
            board[start[0]][start[1]] = State::kStart;
            board[goal[0]][goal[1]] = State::kFinish;
            return board;
        } 
        ExpandNeighbors(current, open, board, goal);
    }
    cout << "No Path Found!";
    return std::vector<vector<State>>{};
}


string cellString(State cell){
    /*
     * evaluates a cell and returns designated value based on integer
     */
    switch(cell){
        case State::kObstacle: return "⛰️  ";
        case State::kPath: return "🚗   ";
        case State::kStart: return "🚦 ";
        case State::kFinish: return "🏁 ";
        default: return "0 ";
    }
}


void printBoard(vector<vector<State>> board){
    /*
     * Prints board and replace values with cellstrings
     */
    
    for (int row=0; row < board.size(); row++){
        for (int column=0; column < board[row].size(); column++){
            cout << cellString(board[row][column]);
        }
        cout << "\n";
    }
}


int main(){
    
    int start[2]{0, 0};
    int goal[2]{4, 5};

    auto board = readBoardFile();
    auto solution = Search(board, start, goal);
    printBoard(solution);
}
