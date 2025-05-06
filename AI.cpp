#include "AI.hpp"
#include "Game.hpp"

AI::AI(Game& game, int player)
    : game(game), player(player)
{
    transpositionTable.clear();
}

static std::string intToString(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

std::string AI::hashBoard() const {
    std::string boardHash;
    std::vector< std::vector<int> > board = game.get_board();
    std::vector< std::vector<int> >::const_iterator rowIt;
    for (rowIt = board.begin(); rowIt != board.end(); ++rowIt) {
        std::vector<int>::const_iterator colIt;
        for (colIt = rowIt->begin(); colIt != rowIt->end(); ++colIt) {
            boardHash += intToString(*colIt);
        }
    }
    return boardHash;
}

std::pair<int, std::pair<int, int> > AI::minimax(int player, int depth, int alpha, int beta, bool isMaximizing) {
    if (depth == 0) {
        int eval = game.evaluate_board(player) + game.get_captures(player) * CAPTURE_WEIGHT;
        return std::make_pair(eval, std::make_pair(-1, -1));
    }
    std::pair<int, int> bestMove = std::make_pair(-1, -1);
    std::vector< std::pair<int, int> > moves = game.get_best_possible_moves(player);
    
    if (isMaximizing) {
        int bestScore = -WIN_WEIGHT;
        for (size_t i = 0; i < moves.size(); i++) {
            int capturesCount = 0;
            std::vector< std::pair<int, int> > capturedStones;
            game.make_move(player, moves[i].first, moves[i].second, capturesCount, capturedStones);
            std::pair<int, std::pair<int, int> > result = minimax(opponent(player), depth - 1, alpha, beta, false);
            int score = result.first + game.get_captures(player) * CAPTURE_WEIGHT;
            game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
            if (score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
            if (score > alpha) { alpha = score; }
            if (beta <= alpha) { break; }
        }
        return std::make_pair(bestScore, bestMove);
    }
    else {
        int bestScore = WIN_WEIGHT;
        for (size_t i = 0; i < moves.size(); i++) {
            int capturesCount = 0;
            std::vector< std::pair<int, int> > capturedStones;
            game.make_move(player, moves[i].first, moves[i].second, capturesCount, capturedStones);
            std::pair<int, std::pair<int, int> > result = minimax(opponent(player), depth - 1, alpha, beta, true);
            int score = -result.first - game.get_captures(player) * CAPTURE_WEIGHT;
            game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
            if (score < bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
            if (score < beta) { beta = score; }
            if (beta <= alpha) { break; }
        }
        return std::make_pair(bestScore, bestMove);
    }
}

void AI::clearTranspositionTable() {
    transpositionTable.clear();
}

std::pair<int, std::pair<int, int> > AI::iterative_deepening(int player, int maxDepth) {
    std::pair<int, std::pair<int, int> > bestMove = std::make_pair(0, std::make_pair(-1, -1));
    int depth = 1;
    while(depth <= maxDepth) {
        bestMove = minimax(player, depth, -WIN_WEIGHT, WIN_WEIGHT, true);
        if (bestMove.first >= WIN_WEIGHT || game.get_captures(player) >= 5) {
            break;
        }
        ++depth;
    }
    return bestMove;
}
