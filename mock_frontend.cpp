#include "AI.hpp"
#include <iostream>
#include <iomanip>

std::vector<std::pair<int,int> > findWinningPositions(const std::vector<std::vector<int> >& board, int winner) {
    int rows = board.size(), cols = board[0].size();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols - 4; c++) {
            if (board[r][c] == winner && board[r][c+1] == winner &&
                board[r][c+2] == winner && board[r][c+3] == winner &&
                board[r][c+4] == winner) {
                return {{r, c}, {r, c+1}, {r, c+2}, {r, c+3}, {r, c+4}};
            }
        }
    }
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows - 4; r++) {
            if (board[r][c] == winner && board[r+1][c] == winner &&
                board[r+2][c] == winner && board[r+3][c] == winner &&
                board[r+4][c] == winner) {
                return {{r, c}, {r+1, c}, {r+2, c}, {r+3, c}, {r+4, c}};
            }
        }
    }
    for (int r = 0; r < rows - 4; r++) {
        for (int c = 0; c < cols - 4; c++) {
            if (board[r][c] == winner && board[r+1][c+1] == winner &&
                board[r+2][c+2] == winner && board[r+3][c+3] == winner &&
                board[r+4][c+4] == winner) {
                return {{r, c}, {r+1, c+1}, {r+2, c+2}, {r+3, c+3}, {r+4, c+4}};
            }
        }
    }
    for (int r = 0; r < rows - 4; r++) {
        for (int c = 4; c < cols; c++) {
            if (board[r][c] == winner && board[r+1][c-1] == winner &&
                board[r+2][c-2] == winner && board[r+3][c-3] == winner &&
                board[r+4][c-4] == winner) {
                return {{r, c}, {r+1, c-1}, {r+2, c-2}, {r+3, c-3}, {r+4, c-4}};
            }
        }
    }
    return {};
}

void printBoard(const Game& game) {
    std::vector<std::vector<int> > board = game.get_board();
    std::cout << "Board state:" << std::endl;
    std::cout << std::setw(3) << " ";
    for (size_t c = 0; c < board[0].size(); ++c) {
        std::cout << std::setw(3) << c;
    }
    std::cout << std::endl;
    
    int winner = EMPTY;
    if (game.is_win(PLAYER1))
        winner = PLAYER1;
    else if (game.is_win(PLAYER2))
        winner = PLAYER2;
    std::vector<std::pair<int,int> > winPositions;
    if (winner != EMPTY) {
        winPositions = findWinningPositions(board, winner);
    }
    
    for (size_t r = 0; r < board.size(); ++r) {
        std::cout << std::setw(3) << r;
        for (size_t c = 0; c < board[r].size(); ++c) {
            bool isWinningCell = false;
            for (std::vector<std::pair<int,int> >::iterator it = winPositions.begin(); it != winPositions.end(); ++it) {
                if ((*it).first == (int)r && (*it).second == (int)c) {
                    isWinningCell = true;
                    break;
                }
            }
            char ch = (board[r][c] == EMPTY) ? '.' :
                      (board[r][c] == PLAYER1 ? 'X' : 'O');
            std::string cellStr(1, ch);
            if (isWinningCell) {
                cellStr = "  \033[1;31m" + cellStr + "\033[0m";
            }
            std::cout << std::setw(3) << cellStr;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {

    Game game;
    AI ai1(game, PLAYER1);
    AI ai2(game, PLAYER2);
    
    int searchDepth = 15;
    
    int currentPlayer = PLAYER1;
    int moveNumber = 0;
    const int maxMoves = BOARD_SIZE * BOARD_SIZE;

    std::cout << "Starting AI vs AI simulation." << std::endl;
    srand(time(nullptr));
    int startRow = rand() % BOARD_SIZE;
    int startCol = rand() % BOARD_SIZE;
    {
        int capturesCount = 0;
        std::vector<std::pair<int,int> > capturedStones;
        if (!game.make_move(currentPlayer, startRow, startCol, capturesCount, capturedStones)) {
            std::cout << "Initial move failed at (" << startRow << ", " << startCol << ")" << std::endl;
            return 1;
        }
        std::cout << "Initial move: Player " << currentPlayer 
             << " (" << (currentPlayer == PLAYER1 ? 'X' : 'O') << ")"
             << " -> (" << startRow << ", " << startCol << ")" << std::endl;
        moveNumber++;
    }
    printBoard(game);
    currentPlayer = (currentPlayer == PLAYER1 ? PLAYER2 : PLAYER1);
    
    while (moveNumber < maxMoves && !game.is_win(PLAYER1) && !game.is_win(PLAYER2)) {
        std::pair<int, std::pair<int, int> > bestMove;
        
        clock_t start = clock();
        if (currentPlayer == PLAYER1) {
            bestMove = ai1.iterative_deepening(PLAYER1, searchDepth);
        } else {
            bestMove = ai2.iterative_deepening(PLAYER2, searchDepth);
        }
        clock_t end = clock();
        long duration = 1000 * (end - start) / CLOCKS_PER_SEC;
        
        int row = bestMove.second.first;
        int col = bestMove.second.second;
        

        if (row < 0 || col < 0) {
            std::cout << "No valid move for player " << currentPlayer << ", terminating." << std::endl;
            break;
        }
        
        int capturesCount = 0;
        std::vector<std::pair<int,int> > capturedStones;
        bool success = game.make_move(currentPlayer, row, col, capturesCount, capturedStones);
        if (!success) {
            std::cout << "Move failed for player " << currentPlayer << " at (" << row << ", " << col << ")" << std::endl;
            break;
        }
        
        std::cout << "Move " << ++moveNumber << ": Player " << currentPlayer 
             << " (" << (currentPlayer == PLAYER1 ? 'X' : 'O') << ")"
             << " -> (" << row << ", " << col << ") captures: " << capturesCount << std::endl;
        std::cout << "Time taken: " << duration << " ms" << std::endl;
        printBoard(game);
        
        currentPlayer = (currentPlayer == PLAYER1 ? PLAYER2 : PLAYER1);
    }
    
    if (game.is_win(PLAYER1)) {
        std::cout << "Player 1 (X) wins!" << std::endl;
    } else if (game.is_win(PLAYER2)) {
        std::cout << "Player 2 (O) wins!" << std::endl;
    } else {
        std::cout << "Game ended in a draw or max moves reached." << std::endl;
    }
    
    return 0;
}
