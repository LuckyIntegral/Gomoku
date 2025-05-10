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
    if (is_win(opp)) return -WIN_WEIGHT;
    
    int score = 0;
    score += countPatternOnBoard(FOUR_UNCOVERED[0], player) * FOUR_UNCOVERED_WEIGHT;
    score -= countPatternOnBoard(FOUR_UNCOVERED[0], opp) * FOUR_UNCOVERED_WEIGHT * 10;
    
    score += get_captures(player) * CAPTURE_WEIGHT;
    score -= get_captures(opp) * CAPTURE_WEIGHT;
    for (std::map<std::vector<int>, int>::const_iterator it = PATTERNS.begin(); it != PATTERNS.end(); ++it) {
        score += countPatternOnBoard(it->first, player) * it->second;
    }
    score -= countPatternOnBoard(THREE_UNCOVERED[0], opp) * THREE_UNCOVERED_WEIGHT * 5;
    
    return score;
}

std::vector<std::pair<int,int> > Game::get_immediate_threats(int player) {
    std::vector<std::pair<int,int> > threats;
    int opp = opponent(player);
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != EMPTY) continue;
            if (would_create_win(opp, i, j))
                threats.emplace_back(i, j);
        }
    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != EMPTY) continue;
            board[i][j] = opp;
            if (countPatternOnBoard(FOUR_UNCOVERED[0], opp) > 0)
                threats.emplace_back(i, j);
            board[i][j] = EMPTY;
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
        moves.insert({BOARD_SIZE / 2, BOARD_SIZE / 2});
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
    int history = moveHistory[row][col];

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

    int dynamicBonus = DYNAMIC_WEIGHT * history;

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

const std::vector<std::pair<int,int> >& Game::getDirections() {
    static const std::vector<std::pair<int,int> > dirs = {
         {-1, -1}, {-1, 0}, {-1, 1},
         {0, -1},  {0, 1},
         {1, -1},  {1, 0}, {1, 1}
    };
    return dirs;
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
                moves.insert({i, j});
            }
        }
    }
}
