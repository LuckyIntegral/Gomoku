#include <atomic>
#include <future>
#include <mutex>
#include <vector>
#include <unordered_map>
#include "Game.hpp"
#include "AI.hpp"

void AI::startBackgroundCalculation() {
    stopFlag.store(false);
    
    // Создаем копию текущего состояния игры
    backgroundGame = std::make_unique<Game>(game);
    
    // Запускаем асинхронный расчет
    backgroundTask = std::async(std::launch::async, &AI::calculateMoves, this);
}

void AI::stopBackgroundCalculation() {
    stopFlag.store(true);
    if (backgroundTask.valid()) {
        backgroundTask.wait();
    }
}

void AI::calculateMoves() {
    currentDepth = 1;
    auto localBest = std::make_pair(-WIN_WEIGHT, std::make_pair(-1, -1));
    
    while (currentDepth <= 10 && !stopFlag.load()) {
        auto [score, move] = minimax(*backgroundGame, currentDepth, 
                                    -WIN_WEIGHT, WIN_WEIGHT, true);
        
        if (score > localBest.first) {
            std::lock_guard<std::mutex> lock(bestMoveMutex);
            localBest = {score, move};
            currentBestMove = localBest;
        }
        
        if (score >= WIN_WEIGHT || stopFlag.load()) break;
        currentDepth++;
    }
}

std::pair<int, int> AI::getBestMove() {
    stopBackgroundCalculation();
    
    std::lock_guard<std::mutex> lock(bestMoveMutex);
    if (currentBestMove.second.first != -1) {
        return currentBestMove.second;
    }
    
    // Fallback: быстрый поиск если фоновая задача не успела
    auto moves = game.getBestPossibleMoves(player);
    return moves.empty() ? std::make_pair(-1, -1) : moves[0];
}

std::pair<int, std::pair<int, int>> AI::minimax(Game& localGame, int depth, 
                                               int alpha, int beta, 
                                               bool isMaximizing) {
    if (stopFlag.load() || depth == 0) {
        return {localGame.heuristicEvaluation(player, -1, -1), {-1, -1}};
    }
    
    std::string boardHash = hashBoard(localGame);
    auto transpositionKey = boardHash + std::to_string(depth) + std::to_string(isMaximizing);
    
    if (transpositionTable.count(transpositionKey)) {
        return transpositionTable[transpositionKey];
    }

    std::pair<int, int> bestMove = {-1, -1};
    auto moves = localGame.getBestPossibleMoves(isMaximizing ? player : 3 - player);
    
    if (moves.empty()) {
        return {localGame.heuristicEvaluation(player, -1, -1), {-1, -1}};
    }

    int bestScore = isMaximizing ? -WIN_WEIGHT : WIN_WEIGHT;
    
    for (const auto& move : moves) {
        if (stopFlag.load()) break;
        
        // Создаем копию игры для ветки расчета
        Game branchGame = localGame;
        int capturesCount;
        std::vector<std::pair<int, int>> capturedStones;
        
        branchGame.makeMove(isMaximizing ? player : 3 - player, 
                          move.first, move.second, 
                          capturesCount, capturedStones);
        
        auto [score, _] = minimax(branchGame, depth - 1, alpha, beta, !isMaximizing);
        
        if (isMaximizing) {
            score += capturesCount * CAPTURE_WEIGHT;
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
                alpha = std::max(alpha, bestScore);
            }
        } else {
            score -= capturesCount * CAPTURE_WEIGHT;
            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
                beta = std::min(beta, bestScore);
            }
        }
        
        if (beta <= alpha || stopFlag.load()) break;
    }

    transpositionTable[transpositionKey] = {bestScore, bestMove};
    return {bestScore, bestMove};
}

std::string AI::hashBoard(const Game& game) const {
    std::string boardHash;
    const auto& board = game.getBoard();
    for (const auto& row : board) {
        for (int cell : row) {
            boardHash += std::to_string(cell);
        }
    }
    return boardHash;
}