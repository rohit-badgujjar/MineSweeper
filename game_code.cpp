#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

#define SIZE 10
#define MINES 10

struct Cell {
    bool isMine;
    bool isRevealed;
    bool isFlagged;
    int neighborMines;
};

vector<vector<Cell>> board(SIZE, vector<Cell>(SIZE));
int minesRemaining = MINES;
int minesFlagged = 0;

void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j].isMine = false;
            board[i][j].isRevealed = false;
            board[i][j].isFlagged = false;
            board[i][j].neighborMines = 0;
        }
    }
}

void plantMines() {
    int count = 0;
    while (count < MINES) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (!board[row][col].isMine) {
            board[row][col].isMine = true;
            count++;
        }
    }
}

void calculateNeighborMines() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (!board[i][j].isMine) {
                int count = 0;
                if (i > 0 && j > 0 && board[i - 1][j - 1].isMine) count++;
                if (i > 0 && board[i - 1][j].isMine) count++;
                if (i > 0 && j < SIZE - 1 && board[i - 1][j + 1].isMine) count++;
                if (j > 0 && board[i][j - 1].isMine) count++;
                if (j < SIZE - 1 && board[i][j + 1].isMine) count++;
                if (i < SIZE - 1 && j > 0 && board[i + 1][j - 1].isMine) count++;
                if (i < SIZE - 1 && board[i + 1][j].isMine) count++;
                if (i < SIZE - 1 && j < SIZE - 1 && board[i + 1][j + 1].isMine) count++;
                board[i][j].neighborMines = count;
            }
        }
    }
}

void revealEmptyCells(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col].isRevealed) {
        return;
    }

    board[row][col].isRevealed = true;

    if (board[row][col].neighborMines == 0) {
        revealEmptyCells(row - 1, col - 1);
        revealEmptyCells(row - 1, col);
        revealEmptyCells(row - 1, col + 1);
        revealEmptyCells(row, col - 1);
        revealEmptyCells(row, col + 1);
        revealEmptyCells(row + 1, col - 1);
        revealEmptyCells(row + 1, col);
        revealEmptyCells(row + 1, col + 1);
    }
}

void printBoard() {
    cout << "\n   ";
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
    }
    cout << "\n\n";
    for (int i = 0; i < SIZE; i++) {
        cout << i << "  ";
        for (int j = 0; j < SIZE; j++) {
            if (!board[i][j].isRevealed) {
                if (board[i][j].isFlagged) {
                    cout << "F ";
                } else {
                    cout << ". ";
                }
            } else if (board[i][j].isMine) {
                cout << "* ";
            } else if (board[i][j].neighborMines > 0) {
                cout << board[i][j].neighborMines << " ";
            } else {
                cout << "  ";
            }
        }
        cout << "\n";
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    cout << "Welcome to Minesweeper!\n"
         << "Instructions:\n"
         << " - The game is played on a grid of cells, some containing hidden mines.\n"
         << " - The objective is to reveal all cells without stepping on a mine.\n"
         << " - Enter your move in the format 'row col action' (e.g., '2 3 R' to reveal cell at row 2, column 3).\n"
         << " - Action 'R' is to reveal a cell, and 'F' is to flag/unflag a cell.\n"
         << " - The grid is labeled with row and column numbers for reference.\n"
         << " - When playing the game, the numbers displayed on the grid indicate the number of neighboring mines for each cell.\n"
         << " - The current state of the board will be displayed.\n"
         << " - Good luck and have fun!\n\n";

    initializeBoard();
    plantMines();
    calculateNeighborMines();

    int row, col;
    char action;

    while (true) {
        printBoard();
        cout << "\nEnter your move (row col action [R - reveal, F - flag]): ";
        cin >> row >> col >> action;

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            cout << "Invalid move! Try again.\n";
            continue;
        }

        if (action == 'R' || action == 'r') {
            if (board[row][col].isMine) {
                cout << "\nGame Over! You stepped on a mine.\n";
                break;
            }
            revealEmptyCells(row, col);
        } else if (action == 'F' || action == 'f') {
            if (board[row][col].isFlagged) {
                board[row][col].isFlagged = false;
                minesRemaining++;
                minesFlagged--;
            } else {
                board[row][col].isFlagged = true;
                minesRemaining--;
                minesFlagged++;
            }
        } else {
            cout << "Invalid action! Try again.\n";
            continue;
        }

        bool allMinesFlagged = true;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j].isMine && !board[i][j].isFlagged) {
                    allMinesFlagged = false;
                    break;
                }
            }
        }

        if (allMinesFlagged) {
            cout << "\nCongratulations! You flagged all the mines. You win!\n";
            break;
        }
    }

    return 0;
}
