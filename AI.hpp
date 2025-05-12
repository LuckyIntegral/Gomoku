#pragma once

#include "Game.hpp"

enum class EntryType { EXACT, LOWER, UPPER };

struct TranspositionEntry {
    int depth;
    int score;
    EntryType type;
    std::pair<int, int> bestMove;
};


struct Descending {
    bool operator()(const std::pair<int, std::pair<int, int> >& a,
                    const std::pair<int, std::pair<int, int> >& b) const {
        return a.first > b.first;
    }
};

struct Ascending {
    bool operator()(const std::pair<int, std::pair<int, int> >& a,
                    const std::pair<int, std::pair<int, int> >& b) const {
        return a.first < b.first;
    }
};

class AI {
    private:
        Game& game;
        int player;
        int lastDepthUsed_;
        int evaluateBoard();
        std::string hashBoard() const;
        std::pair<int, std::pair<int, int> > minimax(int player, int depth, int alpha, int beta, bool isMaximizing,
        const std::chrono::high_resolution_clock::time_point &start, int timeLimit);
        void clearTranspositionTable();
        void initializeZobrist();
        uint64_t computeZobristKey() const;
    public:
        AI(Game& game, int player);
        std::pair<int, std::pair<int, int> > iterative_deepening(int player, int maxDepth);
        int get_last_depth() const;
};
