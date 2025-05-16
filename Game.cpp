#include "Game.hpp"

std::vector<std::vector<uint64_t> > zobristTable;
static std::vector<std::vector<int>> moveHistory;

uint64_t Game::zobristTurn[3] = { 0, 0, 0 };

Game::Game() : player1Capture(0), player2Capture(0) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            board[i][j] = EMPTY;
        }
    }
    if (moveHistory.empty()) {
        moveHistory.resize(BOARD_SIZE, std::vector<int>(BOARD_SIZE, 0));
    }
    initializeZobrist();
    overlineRule = false;

    lastMovePlayer = 0;
    captureStreak[PLAYER1] = 0;
    captureStreak[PLAYER2] = 0;
}

void Game::initializeZobrist() {
    std::mt19937_64 rng(42);
    std::uniform_int_distribution<uint64_t> dist;

    zobristTable.resize(BOARD_SIZE, std::vector<uint64_t>(BOARD_SIZE * 3, 0));
    zobristTurn[1] = dist(rng);
    zobristTurn[2] = dist(rng);

    zobristKey = 0;
    for (size_t i = 0; i < BOARD_SIZE; i++) {
        for (size_t j = 0; j < BOARD_SIZE; j++) {
            int cell = board[i][j];
            zobristKey ^= zobristTable[i][j * 3 + cell];
        }
    }
    zobristKey ^= zobristTurn[PLAYER1];
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
        (player == PLAYER2 && player2Capture >= 5))
        return true;
    
    int dirs[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == player) {
                for (int d = 0; d < 4; d++) {
                    int count = 1;
                    for (int k = 1; k < 5; k++) {
                        int ni = i + k * dirs[d][0], nj = j + k * dirs[d][1];
                        if (ni < 0 || ni >= BOARD_SIZE || nj < 0 || nj >= BOARD_SIZE)
                            break;
                        if (board[ni][nj] == player)
                            count++;
                        else
                            break;
                    }
                    if (count >= 5) {
                        if (overlineRule && count > 5)
                            continue;
                        return true;
                    }
                }
            }
        }
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
    
    int oldVal = board[row][col];
    zobristKey ^= zobristTable[row][col * 3 + oldVal];
    board[row][col] = player;
    zobristKey ^= zobristTable[row][col * 3 + player];

    moveHistory[row][col]++;

    for (std::vector<std::pair<int, int> >::iterator it = capturedStones.begin(); it != capturedStones.end(); ++it) {
        int r = it->first, c = it->second;
        int opp = opponent(player);
        zobristKey ^= zobristTable[r][c * 3 + opp];
        board[r][c] = EMPTY;
        zobristKey ^= zobristTable[r][c * 3 + EMPTY];
    }
    
    zobristKey ^= zobristTurn[player];
    int newTurn = opponent(player); 
    zobristKey ^= zobristTurn[newTurn];

    for (std::vector<std::pair<int, int> >::const_iterator it = capturedStones.begin(); it != capturedStones.end(); ++it) {
        occupiedPositions.erase(*it);
    }

    occupiedPositions.insert(std::make_pair(row, col));
    
    if (player == PLAYER1) {
        player1Capture += capturesCount;
    } else {
        player2Capture += capturesCount;
    }
    
    if (capturesCount > 0) {
        if (lastMovePlayer == player)
            captureStreak[player]++;
        else
            captureStreak[player] = 1;
    } else {
        captureStreak[player] = 0;
    }
    lastMovePlayer = player;
    
    return true;
}

void Game::undo_move(int player, int row, int col, const std::vector<std::pair<int, int> >& capturedStones) {
    int newTurn = opponent(player);
    zobristKey ^= zobristTurn[newTurn];
    zobristKey ^= zobristTurn[player];
    
    int currentVal = board[row][col];
    zobristKey ^= zobristTable[row][col * 3 + currentVal];
    board[row][col] = EMPTY;
    zobristKey ^= zobristTable[row][col * 3 + EMPTY];

    moveHistory[row][col]--;

    for (std::vector<std::pair<int, int> >::iterator it = const_cast<std::vector<std::pair<int, int> >&>(capturedStones).begin();
         it != const_cast<std::vector<std::pair<int, int> >&>(capturedStones).end(); ++it) {
        int r = it->first, c = it->second;
        int opp = opponent(player);
        zobristKey ^= zobristTable[r][c * 3 + EMPTY];
        board[r][c] = opp;
        zobristKey ^= zobristTable[r][c * 3 + opp];
    }

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
    int opp = opponent(player);
    if (is_win(player)) return WIN_WEIGHT;
    if (is_win(opp))    return -WIN_WEIGHT;

    int score = 0;

    for (std::vector<std::vector<int> >::const_iterator it = FOUR_UNCOVERED.begin(); it != FOUR_UNCOVERED.end(); ++it) {
        score += countPatternOnBoard(*it, player) * FOUR_UNCOVERED_WEIGHT;
        score -= countPatternOnBoard(*it, opp) * FOUR_UNCOVERED_WEIGHT * 10;
    }

    for (std::vector<std::vector<int> >::const_iterator it = THREE_UNCOVERED.begin(); it != THREE_UNCOVERED.end(); ++it) {
        score += countPatternOnBoard(*it, player) * THREE_UNCOVERED_WEIGHT;
        score -= countPatternOnBoard(*it, opp) * THREE_UNCOVERED_WEIGHT * 5;
    }

    for (std::vector<std::vector<int> >::const_iterator it = TWO_UNCOVERED.begin(); it != TWO_UNCOVERED.end(); ++it) {
        score += countPatternOnBoard(*it, player) * TWO_UNCOVERED_WEIGHT;
        score -= countPatternOnBoard(*it, opp) * TWO_UNCOVERED_WEIGHT;
    }

    score += get_captures(player) * CAPTURE_WEIGHT;
    score -= get_captures(opp) * CAPTURE_WEIGHT;

    score += dynamic_evaluation(player) - dynamic_evaluation(opp);
    score += frontier_evaluation(player) * FRONTIER_WEIGHT - frontier_evaluation(opp) * FRONTIER_WEIGHT;

    int center = BOARD_SIZE / 2;
    int bonus = 0;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c)
            if (board[r][c] == player)
                bonus += (BOARD_SIZE - (abs(r-center) + abs(c-center)));
    }
    bonus += potential_capture_bonus(player) - potential_capture_bonus(opp);
    
    return score + bonus;
}

int Game::potential_capture_bonus(int player) const {
    static const int POTENTIAL_CAPTURE_WEIGHT = 10;
    int bonus = 0;
    int opp = opponent(player);
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] != EMPTY)
                continue;
            int captures = 0;
            const std::vector<std::pair<int,int> >& dirs = getDirections();
            for (std::vector<std::pair<int,int> >::const_iterator it = dirs.begin(); it != dirs.end(); ++it) {
                int nr1 = r + it->first, nc1 = c + it->second;
                int nr2 = r + 2 * it->first, nc2 = c + 2 * it->second;
                int nr3 = r + 3 * it->first, nc3 = c + 3 * it->second;
                if (nr1 < 0 || nr1 >= BOARD_SIZE || nc1 < 0 || nc1 >= BOARD_SIZE ||
                    nr2 < 0 || nr2 >= BOARD_SIZE || nc2 < 0 || nc2 >= BOARD_SIZE ||
                    nr3 < 0 || nr3 >= BOARD_SIZE || nc3 < 0 || nc3 >= BOARD_SIZE)
                    continue;
                if (board[nr1][nc1] == opp && board[nr2][nc2] == opp && board[nr3][nc3] == EMPTY)
                    ++captures;
            }
            bonus += captures;
        }
    }
    return bonus * POTENTIAL_CAPTURE_WEIGHT;
}

int Game::dynamic_evaluation(int player) const {
    int bonus = 0;
    int center = BOARD_SIZE/2;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c)
            if (board[r][c]==player)
                bonus += (BOARD_SIZE - (abs(r-center)+abs(c-center)));
    }
    return bonus;
}

int Game::frontier_evaluation(int player) const {
    int frontier = 0;
    const std::vector<std::pair<int, int> >& dirs = getDirections();
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (board[r][c] != player) continue;
            for (std::vector<std::pair<int, int> >::const_iterator d = dirs.begin();
                 d != dirs.end(); ++d) {
                int nr = r + d->first, nc = c + d->second;
                if (nr >= 0 && nr < BOARD_SIZE && nc >= 0 && nc < BOARD_SIZE &&
                    board[nr][nc] == EMPTY)
                    ++frontier;
            }
        }
    }
    return frontier;
}

std::vector<std::pair<int,int> > Game::get_immediate_threats(int player) {
    std::vector<std::pair<int,int> > threats;
    int opp = opponent(player);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != EMPTY) continue;
            if (would_create_win(opp, i, j))
                threats.push_back(std::make_pair(i, j));
        }
    }
    return threats;
}

std::vector<std::pair<int, int> > Game::get_best_possible_moves(int player) {
    std::set<std::pair<int, int> > moves;
    for (std::set<std::pair<int, int> >::iterator it = occupiedPositions.begin(); it != occupiedPositions.end(); ++it) {
        for (int dx = -EXPANDED_RADIUS; dx <= EXPANDED_RADIUS; dx++) {
            for (int dy = -EXPANDED_RADIUS; dy <= EXPANDED_RADIUS; dy++) {
                int nx = it->first + dx;
                int ny = it->second + dy;
                if (nx >= 0 && nx < BOARD_SIZE && ny >= 0 && ny < BOARD_SIZE && board[nx][ny] == EMPTY)
                    moves.insert(std::make_pair(nx, ny));
            }
        }
    }
    find_potential_wins(player, moves);
    find_potential_wins(opponent(player), moves);
    if (moves.empty()) {
        moves.insert(std::make_pair(BOARD_SIZE / 2, BOARD_SIZE / 2));
    }
    return std::vector<std::pair<int, int> >(moves.begin(), moves.end());
}

bool Game::matchesThreeInDirection(int player, int row, int col, int dr, int dc) const {
    int negCount = 0;
    for (int i = 1; i <= 2; ++i) {
        int r = row - i * dr, c = col - i * dc;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != player)
            break;
        negCount++;
    }
    int posCount = 0;
    for (int i = 1; i <= 2; ++i) {
        int r = row + i * dr, c = col + i * dc;
        if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE || board[r][c] != player)
            break;
        posCount++;
    }
    int total = negCount + posCount + 1;
    if (total != 3)
        return false;
    
    int start_r = row - negCount * dr;
    int start_c = col - negCount * dc;
    int end_r = row + posCount * dr;
    int end_c = col + posCount * dc;
    
    int before_r = start_r - dr;
    int before_c = start_c - dc;
    if (before_r >= 0 && before_r < BOARD_SIZE && before_c >= 0 && before_c < BOARD_SIZE) {
        if (board[before_r][before_c] != EMPTY)
            return false;
    }
    
    int after_r = end_r + dr;
    int after_c = end_c + dc;
    if (after_r >= 0 && after_r < BOARD_SIZE && after_c >= 0 && after_c < BOARD_SIZE) {
        if (board[after_r][after_c] != EMPTY)
            return false;
    }
    
    return true;
}

bool Game::is_valid_move(int player, int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return false;
    if (board[row][col] != EMPTY)
        return false;
    
    int newThrees = 0;
    
    board[row][col] = player;
    for (std::vector<std::pair<int, int> >::const_iterator d = getLineDirections().begin();
         d != getLineDirections().end(); ++d) {
        if (matchesThreeInDirection(player, row, col, d->first, d->second)) {
            newThrees++;
            if (newThrees >= 2) {
                board[row][col] = EMPTY;
                return false;
            }
        }
    }
    board[row][col] = EMPTY;
    return true;
}

int Game::heuristic_evaluation(int player, int row, int col) {
    int history = moveHistory[row][col];
    double decayFactor = 0.9;
    double dynamicHistory = history * pow(decayFactor, history);
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

    int dynamicBonus = static_cast<int>(DYNAMIC_WEIGHT * dynamicHistory);
    return playerScoreDelta + opponentScoreDelta + dynamicBonus;
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
            capturedStones.insert(std::make_pair(r1, c1));
            capturedStones.insert(std::make_pair(r2, c2));
        }
    }
    return std::make_pair(count, capturedStones);
}

int Game::countPatternInDirection(const std::vector<int>& pattern, int row, int col, int dr, int dc) const {
    int m = pattern.size();
    if (row + (m - 1) * dr < 0 || row + (m - 1) * dr >= BOARD_SIZE)
        return 0;
    if (col + (m - 1) * dc < 0 || col + (m - 1) * dc >= BOARD_SIZE)
        return 0;
    for (int k = 0; k < m; ++k) {
        if (board[row + k * dr][col + k * dc] != pattern[k])
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

const std::vector<std::pair<int,int> >& Game::getDirections() {
    static const std::vector<std::pair<int,int> > dirs = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
    };
    return dirs;
}

const std::vector<std::pair<int,int> >& Game::getLineDirections() {
    static const std::vector<std::pair<int,int> > lineDirs = {
        {0, 1}, {1, 0}, {1, 1}, {1, -1}
    };
    return lineDirs;
}

bool Game::would_create_win(int player, int row, int col) const {
    int oldVal = board[row][col];
    int &cell = const_cast<int&>(board[row][col]);
    cell = player;
    bool win = is_win(player);
    cell = oldVal;
    return win;
}

void Game::find_potential_wins(int player, std::set<std::pair<int,int> >& moves) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] != EMPTY) continue;
            if (would_create_win(player, i, j)) {
                moves.insert(std::make_pair(i, j));
            }
        }
    }
}
