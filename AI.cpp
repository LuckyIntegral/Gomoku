#include "AI.hpp"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <sstream>
#include "Constants.hpp"

template<typename Tuple, std::size_t... Is>
std::string tupleToStringImpl(const Tuple& t, std::index_sequence<Is...>) {
    std::ostringstream oss;
    ((oss << (Is == 0 ? "" : ", ") << std::get<Is>(t)), ...);
    return oss.str();
}

template<typename... Args>
std::string tupleToString(const std::tuple<Args...>& t) {
    return tupleToStringImpl(t, std::index_sequence_for<Args...>{});
}

AI::AI(Game& game, int player) : game(game), player(player) {}

int AI::evaluateBoard() {
    int score = 0;
    for (const auto& [pattern, weight] : PATTERNS) {
        score += game.countPatternOnBoard(pattern, player) * weight;
        score -= game.countPatternOnBoard(pattern, 3 - player) * weight;
    }
    return score;
}

std::string AI::hashBoard() const {
    std::string boardHash;
    for (const auto& row : game.getBoard()) {
        for (int cell : row) {
            boardHash += std::to_string(cell);
        }
    }
    return boardHash;
}

std::pair<int, std::pair<int, int>> AI::minimax(int player, int depth, int alpha, int beta, bool isMaximizing) {
    std::string boardHash = hashBoard();
    auto transpositionKey = std::make_tuple(boardHash, depth, isMaximizing);
    std::string transpositionKeyStr = tupleToString(transpositionKey);
    if (transpositionTable.find(transpositionKeyStr) != transpositionTable.end()) {
        return transpositionTable[transpositionKeyStr];
    }

    if (depth == 0) {
        return {evaluateBoard(), {-1, -1}};
    }

    std::pair<int, int> bestMove = {-1, -1};
    std::vector<std::pair<int, int>> moves = game.getForcedMoves(player);
    if (moves.empty()) {
        moves = game.getBestPossibleMoves(player);
    } else {
        return {0, moves[0]};
    }

    // Move ordering: prioritize moves based on a heuristic
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return game.heuristicEvaluation(player, a.first, a.second) > game.heuristicEvaluation(player, b.first, b.second);
    });

    if (isMaximizing) {
        int bestScore = -WIN_WEIGHT;
        for (size_t i = 0; i < moves.size() && i < 3; ++i) {
            const auto& move = moves[i];
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
        transpositionTable[transpositionKeyStr] = {bestScore, bestMove};
        return {bestScore, bestMove};
    } else {
        int bestScore = WIN_WEIGHT;
        for (size_t i = 0; i < moves.size() && i < 3; ++i) {
            const auto& move = moves[i];
            int capturesCount;
            std::vector<std::pair<int, int>> capturedStones;
            game.makeMove(player, move.first, move.second, capturesCount, capturedStones);
            auto [score, _] = minimax(3 - player, depth - 1, alpha, beta, true);
            score -= capturesCount * CAPTURE_WEIGHT;
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
        transpositionTable[transpositionKeyStr] = {bestScore, bestMove};
        return {bestScore, bestMove};
    }
}

std::pair<int, std::pair<int, int>> AI::iterativeDeepening(int player, int maxDepth) {
    std::cout << "Depth: " << maxDepth << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    auto [bestScore, bestMove] = minimax(player, 1, -WIN_WEIGHT, WIN_WEIGHT, true);
    // for (int i = 2; i <= maxDepth; ++i)
    // {
    //     std::tie(bestScore, bestMove) = minimax(player, i, -WIN_WEIGHT, WIN_WEIGHT, true);
    //     std::cout << i << std::endl;
    // }
    std::tie(bestScore, bestMove) = minimax(player, maxDepth, -WIN_WEIGHT, WIN_WEIGHT, true);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "iterativeDeepening (depth " << maxDepth << ") took " << elapsed.count() << " seconds" << std::endl;
    return {bestScore, bestMove};
}
