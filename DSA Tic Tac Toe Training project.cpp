#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
using namespace std;

class TicTacToe {
public:
    TicTacToe();
    void playGame();
    void displayScores();
    void resetBoard();  // Moved to public

private:
    vector<vector<char>> board;
    char currentPlayer;
    bool playAgainstAI;  // Added to track if playing against AI
    unordered_map<string, int> scores;
    void printBoard();
    bool checkWin();
    bool checkDraw();
    void playerMove();
    void aiMove();
    void switchPlayer();
    void printInstructions();
    void updateScore();
    void initializePlayers();
    pair<int, int> getRowCol(int position);
    int minimax(int depth, bool isMax);
    int evaluateBoard();
    void chooseGameMode();  // Added to choose game mode
};

TicTacToe::TicTacToe() : board(3, vector<char>(3, ' ')), currentPlayer('X'), playAgainstAI(false) {
    initializePlayers();
}

void TicTacToe::initializePlayers() {
    scores["Player 1 (X)"] = 0;
    scores["Player 2 (O)"] = 0;
}

void TicTacToe::printInstructions() {
    // Clear the console (platform-specific)
    #ifdef _WIN32
        system("cls");
    #else
        (void) system("clear");
    #endif

    // Print instructions
    cout << "Welcome to Tic-Tac-Toe!\n";
    cout << "Players take turns to place their mark (X or O) on the board.\n";
    cout << "The first player to get 3 marks in a row (horizontally, vertically, or diagonally) wins.\n";
    cout << "To make a move, enter a number from 1 to 9 corresponding to the board position as follows:\n";
    cout << " 1 | 2 | 3 \n";
    cout << "---|---|---\n";
    cout << " 4 | 5 | 6 \n";
    cout << "---|---|---\n";
    cout << " 7 | 8 | 9 \n";
    cout << "Let's start!\n";

    // Consume any extra newline characters
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void TicTacToe::printBoard() {
    // Clear the console (platform-specific)
    #ifdef _WIN32
        system("cls");
    #else
        (void) system("clear");
    #endif

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                cout << " " << (i * 3 + j + 1) << " ";
            } else {
                cout << " " << board[i][j] << " ";
            }
            if (j < 2) cout << "|";
        }
        cout << endl;
        if (i < 2) cout << "---|---|---" << endl;
    }
}

bool TicTacToe::checkWin() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer)
            return true;
        if (board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer)
            return true;
    }
    if (board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer)
        return true;
    if (board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer)
        return true;
    return false;
}

bool TicTacToe::checkDraw() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (board[i][j] == ' ')
                return false;
    return true;
}

pair<int, int> TicTacToe::getRowCol(int position) {
    int row = (position - 1) / 3;
    int col = (position - 1) % 3;
    return make_pair(row, col);
}

void TicTacToe::playerMove() {
    int position;
    while (true) {
        cout << "Player " << currentPlayer << ", enter your move (1-9): ";
        cin >> position;
        if (cin.fail() || position < 1 || position > 9) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid move. Try again." << endl;
        } else {
            pair<int, int> positionPair = getRowCol(position);
            int row = positionPair.first;
            int col = positionPair.second;
            if (board[row][col] != ' ') {
                cout << "Invalid move. Try again." << endl;
            } else {
                board[row][col] = currentPlayer;
                break;
            }
        }
    }
}

void TicTacToe::aiMove() {
    int bestVal = -1000;
    int bestMove = -1;

    for (int i = 1; i <= 9; ++i) {
        pair<int, int> rc = getRowCol(i);
        int row = rc.first;
        int col = rc.second;
        if (board[row][col] == ' ') {
            board[row][col] = 'O';
            int moveVal = minimax(0, false);
            board[row][col] = ' ';
            if (moveVal > bestVal) {
                bestMove = i;
                bestVal = moveVal;
            }
        }
    }

    pair<int, int> rc = getRowCol(bestMove);
    int row = rc.first;
    int col = rc.second;
    board[row][col] = 'O';
}

void TicTacToe::switchPlayer() {
    currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
}

void TicTacToe::resetBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
    currentPlayer = 'X';
}

void TicTacToe::updateScore() {
    string playerKey = (currentPlayer == 'X') ? "Player 1 (X)" : "Player 2 (O)";
    scores[playerKey]++;
}

void TicTacToe::displayScores() {
    cout << "\nScores:\n";
    for (const auto& pair : scores) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

int TicTacToe::evaluateBoard() {
    for (int i = 0; i < 3; ++i) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == 'O') return +10;
            else if (board[i][0] == 'X') return -10;
        }
    }
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == 'O') return +10;
            else if (board[0][i] == 'X') return -10;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == 'O') return +10;
        else if (board[0][0] == 'X') return -10;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == 'O') return +10;
        else if (board[0][2] == 'X') return -10;
    }
    return 0;
}

int TicTacToe::minimax(int depth, bool isMax) {
    int score = evaluateBoard();

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    if (checkDraw()) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'O';
                    best = max(best, minimax(depth + 1, !isMax));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = 'X';
                    best = min(best, minimax(depth + 1, !isMax));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void TicTacToe::chooseGameMode() {
    char choice;
    cout << "Do you want to play against the AI? (y/n): ";
    cin >> choice;
    playAgainstAI = (choice == 'y' || choice == 'Y');
}

void TicTacToe::playGame() {
    chooseGameMode();
    printInstructions();
    while (true) {
        printBoard();
        if (currentPlayer == 'X' || !playAgainstAI) {
            playerMove();
        } else {
            aiMove();
        }
        if (checkWin()) {
            printBoard();
            cout << "Player " << currentPlayer << " wins!" << endl;
            updateScore();
            displayScores();
            break;
        }
        if (checkDraw()) {
            printBoard();
            cout << "It's a draw!" << endl;
            displayScores();
            break;
        }
        switchPlayer();
    }
}

int main() {
    TicTacToe game;
    char choice;
    do {
        game.playGame();
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y')
            game.resetBoard();
    } while (choice == 'y' || choice == 'Y');
    return 0;
}
