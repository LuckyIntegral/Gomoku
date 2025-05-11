#include "AI.hpp"

extern std::vector<std::vector<uint64_t>> zobristTable;
std::unordered_map<uint64_t, TranspositionEntry> transpositionTable;

static std::string intToString(int n) {
    std::stringstream ss;
    ss << n;
    return ss.str();
}

static inline uint64_t getFullKey(const Game& game) {
    return game.get_zobrist_key();
}

AI::AI(Game& game, int player)
    : game(game), player(player)
{
    transpositionTable.clear();
    initializeZobrist();
}

void AI::initializeZobrist() {
    if (!zobristTable.empty()) return;
    std::mt19937_64 rng(42);
    int rows = game.get_board().size();
    int cols = (rows > 0) ? game.get_board()[0].size() : 0;
    zobristTable.resize(rows, std::vector<uint64_t>(cols * 3));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            for (int k = 0; k < 3; ++k) {
                zobristTable[i][j * 3 + k] = rng();
            }
        }
    }
}

uint64_t AI::computeZobristKey() const {
    uint64_t key = 0;
    std::vector<std::vector<int> > board = game.get_board();
    for (size_t i = 0; i < board.size(); ++i) {
        for (size_t j = 0; j < board[i].size(); ++j) {
            int cell = board[i][j];
            key ^= zobristTable[i][j * 3 + cell];
        }
    }
    return key;
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

std::pair<int, std::pair<int, int> > AI::minimax(int player, int depth, int alpha, int beta, bool isMaximizing,
    const std::chrono::high_resolution_clock::time_point &start, int timeLimit) {
    if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count() > timeLimit) {
        int eval = game.evaluate_board(this->player) + game.get_captures(this->player) * CAPTURE_WEIGHT;
        return {eval, {-1, -1}};
    }

    uint64_t key = getFullKey(game);
    std::unordered_map<uint64_t, TranspositionEntry>::iterator ttIt = transpositionTable.find(key);
    if (ttIt != transpositionTable.end()) {
        TranspositionEntry& entry = ttIt->second;
        if (entry.depth >= depth && (
            entry.type == EntryType::EXACT ||
            (entry.type == EntryType::LOWER && entry.score >= beta) ||
            (entry.type == EntryType::UPPER && entry.score <= alpha))) {
            return { entry.score, entry.bestMove };
        }
    }

    if (depth == 0) {
        int eval = game.evaluate_board(this->player) + game.get_captures(this->player) * CAPTURE_WEIGHT;
        TranspositionEntry entry {0, eval, EntryType::EXACT, std::make_pair(-1,-1)};
        transpositionTable[key] = entry;
        return {eval, std::make_pair(-1, -1)};
    }
    
    std::pair<int, int> bestMove = std::make_pair(-1, -1);
    std::vector<std::pair<int, int> > moves = game.get_best_possible_moves(player);
    
    std::vector<std::pair<int, std::pair<int, int> > > scoredMoves;
    for (std::vector<std::pair<int, int> >::iterator it = moves.begin(); it != moves.end(); ++it) {
        int score = game.heuristic_evaluation(player, it->first, it->second);
        if (game.would_create_win(opponent(player), it->first, it->second)) {
            score += FOUR_UNCOVERED_WEIGHT * 2;
        }
        scoredMoves.push_back(std::make_pair(score, *it));
    }

    if (isMaximizing) {
        std::sort(scoredMoves.begin(), scoredMoves.end(), Descending());
    } else {
        std::sort(scoredMoves.begin(), scoredMoves.end(), Ascending());
    }
    moves.clear();
    for (std::vector<std::pair<int, std::pair<int, int> > >::iterator it = scoredMoves.begin(); it != scoredMoves.end(); ++it)
        moves.push_back(it->second);
    
    {
        std::vector<std::pair<int, int>> winMoves;
        for (std::vector<std::pair<int, int>>::iterator it = moves.begin(); it != moves.end(); ++it) {
            if (game.would_create_win(player, it->first, it->second))
                winMoves.push_back(*it);
        }
        if (!winMoves.empty()) {
            return { WIN_WEIGHT, winMoves[0] };
        }
        std::vector<std::pair<int, int>> threatMoves = game.get_immediate_threats(player);
        if (!threatMoves.empty()) {
            return { WIN_WEIGHT, threatMoves[0] };
        }
    }

    int bestScore = isMaximizing ? -WIN_WEIGHT : WIN_WEIGHT;
    int alphaOrig = alpha;
    if (isMaximizing) {
        for (size_t i = 0; i < moves.size(); i++) {
            int capturesCount = 0;
            std::vector<std::pair<int, int>> capturedStones;
            game.make_move(player, moves[i].first, moves[i].second, capturesCount, capturedStones);
            
            if (game.is_win(opponent(player))) {
                game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
                continue;
            }
            std::pair<int, std::pair<int, int> > result = minimax(opponent(player), depth - 1, alpha, beta, false, start, timeLimit);
            int score = result.first;
            game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
            alpha = std::max(alpha, score);
            if (beta <= alpha) break;
        }
    } else {
        for (size_t i = 0; i < moves.size(); i++) {
            int capturesCount = 0;
            std::vector<std::pair<int, int>> capturedStones;
            game.make_move(player, moves[i].first, moves[i].second, capturesCount, capturedStones);
            
            if (game.is_win(opponent(player))) {
                game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
                continue;
            }
            std::pair<int, std::pair<int, int> > result = minimax(opponent(player), depth - 1, alpha, beta, true, start, timeLimit);
            int score = result.first;
            game.undo_move(player, moves[i].first, moves[i].second, capturedStones);
            
            if (score < bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
            beta = std::min(beta, score);
            if (beta <= alpha) break;
        }
    }
    
    TranspositionEntry entry;
    entry.depth = depth;
    entry.score = bestScore;
    entry.bestMove = bestMove;
    if (bestScore <= alphaOrig) {
        entry.type = EntryType::UPPER;
    } else if (bestScore >= beta) {
        entry.type = EntryType::LOWER;
    } else {
        entry.type = EntryType::EXACT;
    }
    transpositionTable[key] = entry;
    
    std::pair<int, std::pair<int, int> > result = {bestScore, bestMove};
    int score = result.first;
    bool dangerous = false;
    if(dangerous) {
        score -= DANGEROUS_PENALTY;
    }
    result.first = score;
    return result;
}

void AI::clearTranspositionTable() {
    transpositionTable.clear();
}

std::pair<int, std::pair<int, int>> AI::iterative_deepening(int player, int maxDepth) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    std::pair<int, std::pair<int, int> > bestMove = std::make_pair(0, std::make_pair(-1, -1));
    
    int moveCount = 0;
    std::vector<std::vector<int> > board = game.get_board();
    for (std::vector<std::vector<int> >::iterator rowIt = board.begin(); rowIt != board.end(); ++rowIt) {
        for (std::vector<int>::iterator cellIt = rowIt->begin(); cellIt != rowIt->end(); ++cellIt) {
            if (*cellIt != EMPTY)
                moveCount++;
        }
    }
    if (moveCount < 4) {
        static const std::vector<std::pair<int,int> > opening_moves = { std::make_pair(9,9), std::make_pair(8,8), std::make_pair(10,10), std::make_pair(7,7) };
        for (std::vector<std::pair<int,int> >::const_iterator it = opening_moves.begin(); it != opening_moves.end(); ++it) {
            if (game.is_valid_move(player, it->first, it->second))
                return std::make_pair(WIN_WEIGHT, *it);
        }
    }
    
    int depth = 1;
    while (depth <= maxDepth) {
        std::chrono::milliseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::high_resolution_clock::now() - start);
        if (elapsed.count() > TIME_LIMIT_MS)
            break;
        clearTranspositionTable();
        bestMove = minimax(player, depth, -WIN_WEIGHT*2, WIN_WEIGHT*2, true, start, TIME_LIMIT_MS);
        if (abs(bestMove.first) >= WIN_WEIGHT)
            break;
        depth += (depth < 5) ? 1 : 2;
    }
    if (bestMove.second.first == -1) {
        std::vector<std::pair<int, int> > moves = game.get_best_possible_moves(player);
        if (!moves.empty())
            bestMove.second = moves[0];
    }
    return bestMove;
}
