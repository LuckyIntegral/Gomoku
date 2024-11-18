#pragma once

#include <array>
#include <vector>
#include <set>
#include <utility>
#include "Constants.hpp"

class Game {
    private:
        std::array<std::array<int, 19>, 19> board;
        int player1Capture;
        int player2Capture;

        bool isCapture(int player, int row, int col) const;
        std::pair<int, std::set<std::pair<int, int>>> countAndRemoveCaptures(int player, int row, int col);
        std::pair<int, std::set<std::pair<int, int>>> countCaptures(int player, int row, int col) const;
        bool checkPatternHorizontal(const std::vector<int>& pattern, int row, int col) const;
        bool checkPatternVertical(const std::vector<int>& pattern, int row, int col) const;
        bool checkPatternRightDiagonal(const std::vector<int>& pattern, int row, int col) const;
        bool checkPatternLeftDiagonal(const std::vector<int>& pattern, int row, int col) const;

        bool isLeftHorizontalCapture(int player, int row, int col) const;
        bool isRightHorizontalCapture(int player, int row, int col) const;
        bool isUpVerticalCapture(int player, int row, int col) const;
        bool isDownVerticalCapture(int player, int row, int col) const;
        bool isLeftUpDiagonalCapture(int player, int row, int col) const;
        bool isRightUpDiagonalCapture(int player, int row, int col) const;
        bool isLeftDownDiagonalCapture(int player, int row, int col) const;
        bool isRightDownDiagonalCapture(int player, int row, int col) const;

    public:
        Game();
        std::array<std::array<int, 19>, 19> getBoard() const;
        bool makeMove(int player, int row, int col, int& capturesCount, std::vector<std::pair<int, int>>& capturedStones);
        void undoMove(int player, int row, int col, const std::vector<std::pair<int, int>>& capturedStones);
        int evaluateBoard(int player) const;
        std::vector<std::pair<int, int>> getBestPossibleMoves(int player);
        bool isValidMove(int player, int row, int col);
        int heuristicEvaluation(int player, int row, int col);
        int countPatternOnBoard(const std::vector<int>& pattern, int player) const;
        std::vector<std::pair<int, int>> getForcedMoves(int player);
        int getCaptures(int player) const;
};
