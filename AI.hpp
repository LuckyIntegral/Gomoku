#pragma once

#include "Game.hpp"
#include <unordered_map>
#include <string>
#include <utility>
#include <future>

class AI {
    public:
        AI(Game& game, int player) 
            : game(game), player(player), stopFlag(false), currentDepth(0) {}
        
        ~AI() {
            stopBackgroundCalculation();
        }
    
        void startBackgroundCalculation();
        void stopBackgroundCalculation();
        std::pair<int, int> getBestMove();
    
    private:
        Game& game;
        int player;
        std::atomic<bool> stopFlag;
        std::unique_ptr<Game> backgroundGame;
        std::pair<int, std::pair<int, int>> currentBestMove;
        std::mutex bestMoveMutex;
        std::future<void> backgroundTask;
        int currentDepth;
        std::unordered_map<std::string, std::pair<int, std::pair<int, int>>> transpositionTable;
    
        void calculateMoves();
        std::pair<int, std::pair<int, int>> minimax(Game& localGame, int depth, 
                                                   int alpha, int beta, 
                                                   bool isMaximizing);
        std::string hashBoard(const Game& game) const;
    };