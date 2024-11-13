#include "Game.hpp"
#include <algorithm>
#include <numeric>

Game::Game() : player1Capture(0), player2Capture(0) {
    for (auto& row : board) {
        row.fill(EMPTY);
    }
}

std::array<std::array<int, 19>, 19> Game::getBoard() const {
    return board;
}

bool Game::isLeftHorizontalCapture(int player, int row, int col) const {
    if (col < 3) return false;
    return (board[row][col - 1] == 3 - player &&
            board[row][col - 2] == 3 - player &&
            board[row][col - 3] == player);
}

bool Game::isRightHorizontalCapture(int player, int row, int col) const {
    if (col > 15) return false;
    return (board[row][col + 1] == 3 - player &&
            board[row][col + 2] == 3 - player &&
            board[row][col + 3] == player);
}

bool Game::isUpVerticalCapture(int player, int row, int col) const {
    if (row < 3) return false;
    return (board[row - 1][col] == 3 - player &&
            board[row - 2][col] == 3 - player &&
            board[row - 3][col] == player);
}

bool Game::isDownVerticalCapture(int player, int row, int col) const {
    if (row > 15) return false;
    return (board[row + 1][col] == 3 - player &&
            board[row + 2][col] == 3 - player &&
            board[row + 3][col] == player);
}

bool Game::isLeftUpDiagonalCapture(int player, int row, int col) const {
    if (row < 3 || col < 3) return false;
    return (board[row - 1][col - 1] == 3 - player &&
            board[row - 2][col - 2] == 3 - player &&
            board[row - 3][col - 3] == player);
}

bool Game::isRightUpDiagonalCapture(int player, int row, int col) const {
    if (row < 3 || col > 15) return false;
    return (board[row - 1][col + 1] == 3 - player &&
            board[row - 2][col + 2] == 3 - player &&
            board[row - 3][col + 3] == player);
}

bool Game::isLeftDownDiagonalCapture(int player, int row, int col) const {
    if (row > 15 || col < 3) return false;
    return (board[row + 1][col - 1] == 3 - player &&
            board[row + 2][col - 2] == 3 - player &&
            board[row + 3][col - 3] == player);
}

bool Game::isRightDownDiagonalCapture(int player, int row, int col) const {
    if (row > 15 || col > 15) return false;
    return (board[row + 1][col + 1] == 3 - player &&
            board[row + 2][col + 2] == 3 - player &&
            board[row + 3][col + 3] == player);
}

bool Game::makeMove(int player, int row, int col, int& capturesCount, std::vector<std::pair<int, int>>& capturedStones) {
    auto [count, stones] = countAndRemoveCaptures(player, row, col);
    capturesCount = count;
    capturedStones = std::vector<std::pair<int, int>>(stones.begin(), stones.end());
    board[row][col] = player;
    if (player == PLAYER1) {
        player1Capture += capturesCount;
    } else {
        player2Capture += capturesCount;
    }
    return true;
}

void Game::undoMove(int player, int row, int col, const std::vector<std::pair<int, int>>& capturedStones) {
    board[row][col] = EMPTY;
    for (const auto& stone : capturedStones) {
        board[stone.first][stone.second] = 3 - player;
    }
    if (player == PLAYER1) {
        player1Capture -= capturedStones.size();
    } else {
        player2Capture -= capturedStones.size();
    }
}

int Game::evaluateBoard(int player) const {
    int score = 0;
    for (const auto& [pattern, weight] : PATTERNS) {
        score += countPatternOnBoard(pattern, player) * weight;
        score -= countPatternOnBoard(pattern, 3 - player) * weight;
    }
    return score;
}

std::vector<std::pair<int, int>> Game::getBestPossibleMoves(int player) {
    std::set<std::pair<int, int>> moves;
    std::vector<std::pair<int, int>> directions = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    for (int row = 0; row < 19; ++row) {
        for (int col = 0; col < 19; ++col) {
            if (board[row][col] != EMPTY) {
                for (const auto& [dr, dc] : directions) {
                    int nr = row + dr, nc = col + dc;
                    if (moves.count({nr, nc})) continue;
                    if (nr >= 0 && nr < 19 && nc >= 0 && nc < 19 && isValidMove(player, nr, nc)) {
                        moves.insert({nr, nc});
                    }
                }
            }
        }
    }
    if (moves.empty()) {
        moves.insert({9, 9});
    }
    return std::vector<std::pair<int, int>>(moves.begin(), moves.end());
}

bool Game::isValidMove(int player, int row, int col) {
    if (row < 0 || row >= 19 || col < 0 || col >= 19) return false;
    if (board[row][col] != EMPTY) return false;
    if (isCapture(player, row, col)) return true;

    int prevCount = 0;
    for (const auto& pattern : THREE_UNCOVERED) {
        prevCount += countPatternOnBoard(pattern, player);
    }
    board[row][col] = player;
    int newCount = 0;
    for (const auto& pattern : THREE_UNCOVERED) {
        newCount += countPatternOnBoard(pattern, player);
    }
    board[row][col] = EMPTY;
    return newCount - prevCount < 2;
}

int Game::heuristicEvaluation(int player, int row, int col) {
    auto [numOfCaptures, capturedPositions] = countCaptures(player, row, col);
    board[row][col] = player;
    for (const auto& [r, c] : capturedPositions) {
        board[r][c] = EMPTY;
    }
    int score = evaluateBoard(player);
    score += numOfCaptures * CAPTURE_WEIGHT;
    board[row][col] = EMPTY;
    for (const auto& [r, c] : capturedPositions) {
        board[r][c] = 3 - player;
    }
    return score;
}

bool Game::isCapture(int player, int row, int col) const {
    return isLeftDownDiagonalCapture(player, row, col) ||
           isRightHorizontalCapture(player, row, col) ||
           isUpVerticalCapture(player, row, col) ||
           isDownVerticalCapture(player, row, col) ||
           isLeftUpDiagonalCapture(player, row, col) ||
           isRightUpDiagonalCapture(player, row, col) ||
           isLeftDownDiagonalCapture(player, row, col) ||
           isRightDownDiagonalCapture(player, row, col);
}

std::pair<int, std::set<std::pair<int, int>>> Game::countAndRemoveCaptures(int player, int row, int col) {
    int count = 0;
    std::set<std::pair<int, int>> capturedStones;
    if (isLeftHorizontalCapture(player, row, col)) {
        count++;
        board[row][col - 1] = EMPTY;
        board[row][col - 2] = EMPTY;
        capturedStones.insert({row, col - 1});
        capturedStones.insert({row, col - 2});
    }
    if (isRightHorizontalCapture(player, row, col)) {
        count++;
        board[row][col + 1] = EMPTY;
        board[row][col + 2] = EMPTY;
        capturedStones.insert({row, col + 1});
        capturedStones.insert({row, col + 2});
    }
    if (isUpVerticalCapture(player, row, col)) {
        count++;
        board[row - 1][col] = EMPTY;
        board[row - 2][col] = EMPTY;
        capturedStones.insert({row - 1, col});
        capturedStones.insert({row - 2, col});
    }
    if (isDownVerticalCapture(player, row, col)) {
        count++;
        board[row + 1][col] = EMPTY;
        board[row + 2][col] = EMPTY;
        capturedStones.insert({row + 1, col});
        capturedStones.insert({row + 2, col});
    }
    if (isLeftUpDiagonalCapture(player, row, col)) {
        count++;
        board[row - 1][col - 1] = EMPTY;
        board[row - 2][col - 2] = EMPTY;
        capturedStones.insert({row - 1, col - 1});
        capturedStones.insert({row - 2, col - 2});
    }
    if (isRightUpDiagonalCapture(player, row, col)) {
        count++;
        board[row - 1][col + 1] = EMPTY;
        board[row - 2][col + 2] = EMPTY;
        capturedStones.insert({row - 1, col + 1});
        capturedStones.insert({row - 2, col + 2});
    }
    if (isLeftDownDiagonalCapture(player, row, col)) {
        count++;
        board[row + 1][col - 1] = EMPTY;
        board[row + 2][col - 2] = EMPTY;
        capturedStones.insert({row + 1, col - 1});
        capturedStones.insert({row + 2, col - 2});
    }
    if (isRightDownDiagonalCapture(player, row, col)) {
        count++;
        board[row + 1][col + 1] = EMPTY;
        board[row + 2][col + 2] = EMPTY;
        capturedStones.insert({row + 1, col + 1});
        capturedStones.insert({row + 2, col + 2});
    }
    return {count, capturedStones};
}

std::pair<int, std::set<std::pair<int, int>>> Game::countCaptures(int player, int row, int col) const {
    int count = 0;
    std::set<std::pair<int, int>> capturedStones;
    if (isLeftHorizontalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row, col - 1});
        capturedStones.insert({row, col - 2});
    }
    if (isRightHorizontalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row, col + 1});
        capturedStones.insert({row, col + 2});
    }
    if (isUpVerticalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row - 1, col});
        capturedStones.insert({row - 2, col});
    }
    if (isDownVerticalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row + 1, col});
        capturedStones.insert({row + 2, col});
    }
    if (isLeftUpDiagonalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row - 1, col - 1});
        capturedStones.insert({row - 2, col - 2});
    }
    if (isRightUpDiagonalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row - 1, col + 1});
        capturedStones.insert({row - 2, col + 2});
    }
    if (isLeftDownDiagonalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row + 1, col - 1});
        capturedStones.insert({row + 2, col - 2});
    }
    if (isRightDownDiagonalCapture(player, row, col)) {
        count++;
        capturedStones.insert({row + 1, col + 1});
        capturedStones.insert({row + 2, col + 2});
    }
    return {count, capturedStones};
}

bool Game::checkPatternHorizontal(const std::vector<int>& pattern, int row, int col) const {
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (col + i >= 19 || board[row][col + i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

bool Game::checkPatternVertical(const std::vector<int>& pattern, int row, int col) const {
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (row + i >= 19 || board[row + i][col] != pattern[i]) {
            return false;
        }
    }
    return true;
}

bool Game::checkPatternDiagonal(const std::vector<int>& pattern, int row, int col) const {
    for (size_t i = 0; i < pattern.size(); ++i) {
        if (row + i >= 19 || col + i >= 19 || board[row + i][col + i] != pattern[i]) {
            return false;
        }
    }
    return true;
}

int Game::countPatternOnBoard(const std::vector<int>& pattern, int player) const {
    std::vector<int> newPattern = pattern;
    if (player == PLAYER2) {
        std::transform(newPattern.begin(), newPattern.end(), newPattern.begin(), [](int cell) {
            return cell == PLAYER1 ? PLAYER2 : (cell == PLAYER2 ? PLAYER1 : EMPTY);
        });
    }
    int count = 0;
    for (int row = 0; row < 19; ++row) {
        for (int col = 0; col < 19; ++col) {
            count += checkPatternHorizontal(newPattern, row, col);
            count += checkPatternVertical(newPattern, row, col);
            count += checkPatternDiagonal(newPattern, row, col);
        }
    }
    return count;
}

std::vector<std::pair<int, int>> Game::getForcedMoves(int player) {
    std::vector<std::pair<int, int>> forcedMoves;
    std::vector<std::pair<int, int>> capturedStones;
    int captureCount = 0;
        // Check if placing a stone here blocks an opponent's winning move
    auto moves = getBestPossibleMoves(3 - player);
    for (auto move : moves)
    {
        makeMove(3 - player, move.first, move.second, captureCount, capturedStones);
        if (evaluateBoard(3 - player) >= THREE_UNCOVERED_WEIGHT * 2) {
            forcedMoves.push_back({move.first, move.second});
        }
        undoMove(3 - player, move.first, move.second, capturedStones);
        capturedStones.clear();
    }

    // Check if placing a stone here continues a sequence that leads to a win
    for (auto move : moves)
    {
        makeMove(player, move.first, move.second, captureCount, capturedStones);
        if (evaluateBoard(player) >= THREE_UNCOVERED_WEIGHT * 2) {
            forcedMoves.push_back({move.first, move.second});
        }
        undoMove(player, move.first, move.second, capturedStones);
        capturedStones.clear();
    }
    return forcedMoves;
}