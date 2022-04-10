#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using std::vector;
using std::cout;
using std::ifstream;
using std::string;
using std::istringstream;


enum class State {kEmpty, kObstacle, kClosed};

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &open_nodes, vector<vector<State>> &grid){
   vector<int> node = {x, y, g, h};
   open_nodes.push_back(node);
   grid[x][y] = State::kClosed;
}
vector<vector<State>> Search(vector<vector<State>> board, int start[2], int goal[2]){
    cout << "No path found!"<< "\n";
    return vector<vector<State>> {};
}


int Heuristic(int x1, int x2, int y1, int y2){
    int distance = (x2-x1) + (y2-y1);
    return distance;
}


vector<State> ParseLine(string word){
    int num;
    char comma;
    vector<State> data;
    istringstream mystream(word);

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

string cellString(State cell){
    switch(cell){
        case State::kObstacle: return "⛰️  ";
        default: return "0 ";
    }
}

void printBoard(vector<vector<State>> board){
    for (int row=0; row < board.size(); row++){
        for (int column=0; column < board[row].size(); column++){
            cout << cellString(board[row][column]);
        }
        cout << "\n";
    }
}


int main(){
    auto board = readBoardFile();
    int start[2] = {0, 0};
    int goal[2] = {4, 5};

    auto solution = Search(board, start, goal);
    cout << start << "\n";
    printBoard(solution);

}
