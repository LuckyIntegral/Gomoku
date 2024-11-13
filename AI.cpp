#include "AI.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include "Constants.hpp"

AI::AI(Game& game, int player) : game(game), player(player) {}

int AI::evaluateBoard() {
    auto start = std::chrono::high_resolution_clock::now();
    int score = 0;
    for (const auto& [pattern, weight] : PATTERNS) {
        score += game.countPatternOnBoard(pattern, player) * weight;
        score -= game.countPatternOnBoard(pattern, 3 - player) * weight;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "evaluateBoard took " << elapsed.count() << " seconds" << std::endl;
    return score;
}

std::string AI::hashBoard() const {
    auto start = std::chrono::high_resolution_clock::now();
    std::string boardHash;
    for (const auto& row : game.getBoard()) {
        for (int cell : row) {
            boardHash += std::to_string(cell);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "hashBoard took " << elapsed.count() << " seconds" << std::endl;
    return boardHash;
}

std::pair<int, std::pair<int, int>> AI::minimax(int player, int depth, int alpha, int beta, bool isMaximizing) {
    auto start = std::chrono::high_resolution_clock::now();
    std::string boardHash = hashBoard();
    auto transpositionKey = std::make_tuple(boardHash, depth, isMaximizing);
    if (transpositionTable.find(boardHash) != transpositionTable.end()) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "minimax (transposition table hit) took " << elapsed.count() << " seconds" << std::endl;
        return transpositionTable[boardHash];
    }

    if (depth == 0) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "minimax (depth 0) took " << elapsed.count() << " seconds" << std::endl;
        return {evaluateBoard(), {-1, -1}};
    }

    std::pair<int, int> bestMove = {-1, -1};
    std::vector<std::pair<int, int>> moves = game.getForcedMoves(player);
    if (moves.empty()) {
        moves = game.getBestPossibleMoves(player);
    }

    // Move ordering: prioritize moves based on a heuristic
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return game.heuristicEvaluation(player, a.first, a.second) > game.heuristicEvaluation(player, b.first, b.second);
    });

    if (isMaximizing) {
        int bestScore = -1000000;
        for (const auto& move : moves) {
            int capturesCount;
            std::vector<std::pair<int, int>> capturedStones;
            game.makeMove(player, move.first, move.second, capturesCount, capturedStones);
            auto [score, _] = minimax(3 - player, depth - 1, alpha, beta, false);
            score += capturesCount * CAPTURE_WEIGHT;
            game.undoMove(player, move.first, move.second, capturedStones);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
            alpha = std::max(alpha, score);
            if (beta <= alpha) {
                break;
            }
        }
        transpositionTable[boardHash] = {bestScore, bestMove};
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "minimax took " << elapsed.count() << " seconds" << std::endl;
        return {bestScore, bestMove};
    } else {
        int bestScore = 1000000;
        for (const auto& move : moves) {
            int capturesCount;
            std::vector<std::pair<int, int>> capturedStones;
            game.makeMove(player, move.first, move.second, capturesCount, capturedStones);
            auto [score, _] = minimax(3 - player, depth - 1, alpha, beta, true);
            score += capturesCount * CAPTURE_WEIGHT;
            game.undoMove(player, move.first, move.second, capturedStones);

            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
            beta = std::min(beta, score);
            if (beta <= alpha) {
                break;
            }
        }
        transpositionTable[boardHash] = {bestScore, bestMove};
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "minimax took " << elapsed.count() << " seconds" << std::endl;
        return {bestScore, bestMove};
    }
}

std::pair<int, std::pair<int, int>> AI::iterativeDeepening(int player, int maxDepth) {
    std::cout << "Depth: " << maxDepth << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    auto [bestScore, bestMove] = minimax(player, maxDepth, -1000000, 1000000, true);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "iterativeDeepening (depth " << maxDepth << ") took " << elapsed.count() << " seconds" << std::endl;
    return {bestScore, bestMove};
}