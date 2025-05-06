#include "Game.hpp"

Game::Game() : player1Capture(0), player2Capture(0) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

std::vector<std::vector<int> > Game::get_board() const {
    std::vector<std::vector<int> > vecBoard(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    int i, j;
    for (i = 0; i < BOARD_SIZE; ++i) {
        for (j = 0; j < BOARD_SIZE; ++j) {
            vecBoard[i][j] = board[i][j];
        }
    }
    return vecBoard;
}

bool Game::is_win(int player) const {
    if ((player == PLAYER1 && player1Capture >= 5) ||
        (player == PLAYER2 && player2Capture >= 5) ||
        (countPatternOnBoard(WIN, player) > 0)) {
        return true;
    }
    return false;
}

bool Game::isCaptureInDirection(int player, int row, int col, int dr, int dc) const {
    int opponentPlayer = opponent(player);
    int pos1Row = row + dr, pos1Col = col + dc;
    int pos2Row = row + 2 * dr, pos2Col = col + 2 * dc;
    int pos3Row = row + 3 * dr, pos3Col = col + 3 * dc;

    if ((pos1Row < 0 || pos1Row >= BOARD_SIZE) || (pos1Col < 0 || pos1Col >= BOARD_SIZE) ||
        (pos2Row < 0 || pos2Row >= BOARD_SIZE) || (pos2Col < 0 || pos2Col >= BOARD_SIZE) ||
        (pos3Row < 0 || pos3Row >= BOARD_SIZE) || (pos3Col < 0 || pos3Col >= BOARD_SIZE)) {
        return false;
    }

    return (board[pos1Row][pos1Col] == opponentPlayer &&
            board[pos2Row][pos2Col] == opponentPlayer &&
            board[pos3Row][pos3Col] == player);
}

bool Game::make_move(int player, int row, int col, int& capturesCount, std::vector<std::pair<int, int> >& capturedStones) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    
    std::pair<int, std::set<std::pair<int, int> > > tmp = countAndRemoveCaptures(player, row, col);
    capturesCount = tmp.first;
    capturedStones.assign(tmp.second.begin(), tmp.second.end());
    
    board[row][col] = player;
    for (std::vector<std::pair<int, int> >::const_iterator it = capturedStones.begin(); it != capturedStones.end(); ++it) {
        occupiedPositions.erase(*it);
    }

    occupiedPositions.insert(std::make_pair(row, col));
    
    if (player == PLAYER1) {
        player1Capture += capturesCount;
    } else {
        player2Capture += capturesCount;
    }
    
    return true;
}

void Game::undo_move(int player, int row, int col, const std::vector<std::pair<int, int> >& capturedStones) {
    board[row][col] = EMPTY;

    for (std::vector<std::pair<int, int> >::const_iterator it = capturedStones.begin(); it != capturedStones.end(); ++it) {
        board[it->first][it->second] = opponent(player);
    }

    int removedCaptures = capturedStones.size() / 2;
    if (player == PLAYER1)
        player1Capture -= removedCaptures;
    else
        player2Capture -= removedCaptures;

    occupiedPositions.erase(std::make_pair(row, col));
    for (std::vector<std::pair<int, int> >::const_iterator it = capturedStones.begin(); it != capturedStones.end(); ++it) {
        occupiedPositions.insert(*it);
    }
}

int Game::evaluate_board(int player) const {
    int score = 0;

    std::map<std::vector<int>, int>::const_iterator it;
    for(it = PATTERNS.begin(); it != PATTERNS.end(); ++it) {
        score += countPatternOnBoard(it->first, player) * it->second;
    }
    return score;
}

std::vector<std::pair<int, int> > Game::get_best_possible_moves(int /*player*/) {
    std::set<std::pair<int, int> > moves;
    const std::vector<std::pair<int, int> >& directions = Game::getDirections();
    
    for (std::set<std::pair<int, int> >::const_iterator pos = occupiedPositions.begin(); pos != occupiedPositions.end(); ++pos) {
        int row = pos->first;
        int col = pos->second;
        for (std::vector<std::pair<int, int> >::const_iterator d = directions.begin(); d != directions.end(); ++d) {
            int nr = row + d->first;
            int nc = col + d->second;
            if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
                board[nr][nc] == EMPTY) {
                moves.insert(std::make_pair(nr, nc));
            }
        }
    }
    
    if (moves.empty()) {
        moves.insert(std::make_pair(BOARD_SIZE/2, BOARD_SIZE/2));
    }
    return std::vector<std::pair<int, int> >(moves.begin(), moves.end());
}

bool Game::is_valid_move(int player, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) return false;
    if (board[row][col] != EMPTY) return false;
    if (isCapture(player, row, col)) return true;

    int prevCount = 0;

    std::vector<std::vector<int> >::const_iterator itPattern;
    for(itPattern = THREE_UNCOVERED.begin(); itPattern != THREE_UNCOVERED.end(); ++itPattern) {
        prevCount += countPatternOnBoard(*itPattern, player);
    }
    board[row][col] = player;
    int newCount = 0;
    for(itPattern = THREE_UNCOVERED.begin(); itPattern != THREE_UNCOVERED.end(); ++itPattern) {
        newCount += countPatternOnBoard(*itPattern, player);
    }
    board[row][col] = EMPTY;
    return newCount - prevCount < 2;
}

int Game::heuristic_evaluation(int player, int row, int col) {
    std::vector<std::pair<int, int> > capturedStones;
    int captureCount = 0;

    int opponentPlayer = opponent(player);

    int playerScoreBefore = evaluate_board(player) + get_captures(player) * CAPTURE_WEIGHT;
    int opponentScoreBefore = evaluate_board(opponentPlayer) + get_captures(opponentPlayer) * CAPTURE_WEIGHT;

    make_move(player, row, col, captureCount, capturedStones);

    int playerScoreAfter = evaluate_board(player) + get_captures(player) * CAPTURE_WEIGHT;
    int opponentScoreAfter = evaluate_board(opponentPlayer) + get_captures(opponentPlayer) * CAPTURE_WEIGHT;

    undo_move(player, row, col, capturedStones);

    int playerScoreDelta = playerScoreAfter - playerScoreBefore;
    int opponentScoreDelta = opponentScoreBefore - opponentScoreAfter;

    return playerScoreDelta + opponentScoreDelta;
}

bool Game::isCapture(int player, int row, int col) const {
    const std::vector<std::pair<int, int> >& directions = Game::getDirections();
    for (std::vector<std::pair<int, int> >::const_iterator d = directions.begin(); d != directions.end(); ++d) {
        if (isCaptureInDirection(player, row, col, d->first, d->second))
            return true;
    }
    return false;
}

std::pair<int, std::set<std::pair<int, int> > > Game::countAndRemoveCaptures(int player, int row, int col) {
    int count = 0;
    std::set<std::pair<int, int> > capturedStones;
    
    const std::vector<std::pair<int, int> >& directions = Game::getDirections();
    for (std::vector<std::pair<int, int> >::const_iterator it = directions.begin(); it != directions.end(); ++it) {
        int dr = it->first, dc = it->second;
        if (isCaptureInDirection(player, row, col, dr, dc)) {
            count++;
            int r1 = row + dr, c1 = col + dc;
            int r2 = row + 2 * dr, c2 = col + 2 * dc;
            board[r1][c1] = EMPTY;
            board[r2][c2] = EMPTY;
            capturedStones.insert(std::make_pair(r1, c1));
            capturedStones.insert(std::make_pair(r2, c2));
        }
    }
    return std::make_pair(count, capturedStones);
}

int Game::countPatternInDirection(const std::vector<int>& pattern, int row, int col, int dr, int dc) const {
    int m = pattern.size();

    int endRow = row + (m - 1) * dr;
    int endCol = col + (m - 1) * dc;
    if(endRow < 0 || endRow >= BOARD_SIZE || endCol < 0 || endCol >= BOARD_SIZE)
        return 0;
    for (int k = 0; k < m; ++k) {
        if(board[row + k * dr][col + k * dc] != pattern[k])
            return 0;
    }
    return 1;
}

int Game::countPatternOnBoard(const std::vector<int>& pattern, int player) const {
    std::vector<int> newPattern(pattern);
    if (player == PLAYER2) {
        for (size_t i = 0; i < newPattern.size(); ++i) {
            if (newPattern[i] == PLAYER1)
                newPattern[i] = PLAYER2;
            else if (newPattern[i] == PLAYER2)
                newPattern[i] = PLAYER1;
        }
    }
    int count = 0;

    std::vector<std::pair<int, int> > directions;
    directions.push_back(std::make_pair(0, 1));
    directions.push_back(std::make_pair(1, 0));
    directions.push_back(std::make_pair(1, 1));
    directions.push_back(std::make_pair(1, -1));
    
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            for (std::vector<std::pair<int, int> >::const_iterator i = directions.begin(); i != directions.end(); ++i) {
                count += countPatternInDirection(newPattern, row, col, i->first, i->second);
            }
        }
    }
    return count;
}

int Game::get_captures(int player) const {
    return (player == PLAYER1) ? player1Capture : player2Capture;
}

const std::vector<std::pair<int,int>>& Game::getDirections() {
    static const std::vector<std::pair<int,int>> dirs = {
         {-1, -1}, {-1, 0}, {-1, 1},
         {0, -1},  {0, 1},
         {1, -1},  {1, 0}, {1, 1}
    };
    return dirs;
}
