#pragma once

#include "Game.hpp"
#include <unordered_map>
#include <string>
#include <utility>

class AI {
    private:
        Game& game;
        int player;
        std::unordered_map<std::string, std::pair<int, std::pair<int, int>>> transpositionTable;

        int evaluateBoard();
        std::string hashBoard() const;
        std::pair<int, std::pair<int, int>> minimax(int player, int depth, int alpha, int beta, bool isMaximizing);
    public:
        AI(Game& game, int player);
        std::pair<int, std::pair<int, int>> iterativeDeepening(int player, int maxDepth);

};
