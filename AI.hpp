#pragma once

#include <map>
#include <string>
#include <utility>
#include <sstream>
#include "Game.hpp"

class AI {
    private:
        Game& game;
        int player;
        std::map<std::string, std::pair<int, std::pair<int, int> > > transpositionTable;

        int evaluateBoard();
        std::string hashBoard() const;
        std::pair<int, std::pair<int, int> > minimax(int player, int depth, int alpha, int beta, bool isMaximizing);
        void clearTranspositionTable();
    public:
        AI(Game& game, int player);
        std::pair<int, std::pair<int, int> > iterative_deepening(int player, int maxDepth);

};
