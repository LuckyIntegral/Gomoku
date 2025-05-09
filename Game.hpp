#pragma once

#include "Constants.hpp"
#include <cstdint>
#include <set>
#include <utility>
#include <cstddef>
#include <random>
#include <string>
#include <sstream>
#include <unordered_map>
#include <chrono>
#include <algorithm>

class Game {
    private:
        int board[BOARD_SIZE][BOARD_SIZE];
        std::set<std::pair<int, int> > occupiedPositions;
        int player1Capture;
        int player2Capture;
        uint64_t zobristKey;
        bool overlineRule;

        static uint64_t zobristTurn[3];

        bool isCapture(int player, int row, int col) const;
        std::pair<int, std::set<std::pair<int, int> > > countAndRemoveCaptures(int player, int row, int col);
        int countPatternOnBoard(const std::vector<int>& pattern, int player) const;
        int countPatternInDirection(const std::vector<int>& pattern, int row, int col, int dr, int dc) const;
        bool isCaptureInDirection(int player, int row, int col, int dr, int dc) const;
        void find_potential_wins(int player, std::set<std::pair<int,int>>& moves);

    public:
        Game();
        std::vector<std::vector<int> > get_board() const;
        bool make_move(int player, int row, int col,
                       int& capturesCount,
                       std::vector<std::pair<int,int> >& capturedStones);
        void undo_move(int player, int row, int col,
                       const std::vector<std::pair<int,int> >& capturedStones);
        int evaluate_board(int player) const;
        std::vector<std::pair<int,int> > get_best_possible_moves(int player);
        bool is_valid_move(int player, int row, int col);
        int heuristic_evaluation(int player, int row, int col);
        int get_captures(int player) const;
        bool is_win(int player) const;
        static const std::vector<std::pair<int,int> >& getDirections();
        uint64_t get_zobrist_key() const { return zobristKey; }
        void initializeZobrist();
        std::vector<std::pair<int,int>> get_immediate_threats(int player);
        bool would_create_win(int player, int row, int col) const;
};
