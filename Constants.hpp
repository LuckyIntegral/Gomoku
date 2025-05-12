#pragma once

#include <vector>
#include <map>

const int BOARD_SIZE = 19;

const int PLAYER1 = 1;
const int PLAYER2 = 2;
const int EMPTY = 0;

const int WIN_WEIGHT = 10000000;
const int FOUR_UNCOVERED_WEIGHT = 9500000;
const int THREE_UNCOVERED_WEIGHT = 100000;
const int TWO_UNCOVERED_WEIGHT = 1;
const int FOUR_COVERED_WEIGHT = 90;
const int THREE_COVERED_WEIGHT = 10;
const int CAPTURE_WEIGHT = 500000;

const int EXPANDED_RADIUS = 5;
const int TIME_LIMIT_MS = 400;
const int DANGEROUS_PENALTY = 5000000;

const int OPPONENT_OFFSET = 3;
inline int opponent(int player) { return OPPONENT_OFFSET - player; }

const int DYNAMIC_WEIGHT = 50;
const int FRONTIER_WEIGHT = 20;
const int CAPTURE_STREAK_WEIGHT = 200000;

extern const std::vector<int> WIN;
extern const std::vector<std::vector<int> > FOUR_UNCOVERED;
extern const std::vector<std::vector<int> > THREE_UNCOVERED;
extern const std::vector<std::vector<int> > TWO_UNCOVERED;
extern const std::vector<std::vector<int> > FOUR_COVERED;
extern const std::map<std::vector<int>, int> PATTERNS;
