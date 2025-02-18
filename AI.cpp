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
    // std::string boardHash = hashBoard();
    // auto transpositionKey = std::make_tuple(boardHash, depth, isMaximizing);
    // std::string transpositionKeyStr = tupleToString(transpositionKey);
    // if (transpositionTable.find(transpositionKeyStr) != transpositionTable.end()) {
    //     std::cout << "Key found" << std::endl;
    //     return transpositionTable[transpositionKeyStr];
    // }

    if (depth == 0) {
        return {game.heuristicEvaluation(player, -1, -1), {-1, -1}};
    }
    std::pair<int, int> bestMove = {-1, -1};
    auto moves = game.getBestPossibleMoves(player);

    // Move ordering: prioritize moves based on a heuristic
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return game.heuristicEvaluation(player, a.first, a.second) > game.heuristicEvaluation(player, b.first, b.second);
    });

    moves = std::vector<std::pair<int, int>>(moves.begin(), moves.begin() + std::min(1, (int)moves.size()));
    if (isMaximizing) {
        int bestScore = -WIN_WEIGHT;
        for (const auto& move : moves) {
            int capturesCount;
            std::vector<std::pair<int, int>> capturedStones;
            game.makeMove(player, move.first, move.second, capturesCount, capturedStones);
            auto [score, _] = minimax(3 - player, depth - 1, alpha, beta, false);
            score += game.getCaptures(player) * CAPTURE_WEIGHT;
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
        //transpositionTable[transpositionKeyStr] = {bestScore, bestMove};
        return {bestScore, bestMove};
    } else {
        int bestScore = WIN_WEIGHT;
        for (const auto& move : moves) {
            int capturesCount;
            std::vector<std::pair<int, int>> capturedStones;
            game.makeMove(player, move.first, move.second, capturesCount, capturedStones);
            auto [score, _] = minimax(3 - player, depth - 1, alpha, beta, true);
            score = -score;
            score -= game.getCaptures(player) * CAPTURE_WEIGHT;
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
        //transpositionTable[transpositionKeyStr] = {bestScore, bestMove};
        return {bestScore, bestMove};
    }
}

void AI::clearTranspositionTable() {
    transpositionTable.clear();
}

std::pair<int, std::pair<int, int>> AI::iterativeDeepening(int player, int maxDepth) {
    std::pair<int, std::pair<int, int>> bestMove = {0, {-1, -1}};
    for (int depth = 1; depth <= maxDepth; ++depth) {
        bestMove = minimax(player, maxDepth, -WIN_WEIGHT, WIN_WEIGHT, true);
        if (bestMove.first >= WIN_WEIGHT || this->game.getCaptures(player) >= 5) {
            break;
        }
    }
    return bestMove;
}
