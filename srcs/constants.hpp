#pragma once

#include <vector>

using namespace std;

extern const long WIN_WEIGHT;
extern const long FOUR_UNCOVERED_WEIGHT;
extern const long THREE_UNCOVERED_WEIGHT;
extern const long TWO_UNCOVERED_WEIGHT;
extern const long FOUR_COVERED_WEIGHT;
extern const long THREE_COVERED_WEIGHT;
extern const long CAPTURE_WEIGHT;

extern vector<vector<u_char>> WIN;
extern vector<vector<u_char>> FOUR_UNCOVERED;
extern vector<vector<u_char>> THREE_UNCOVERED;
extern vector<vector<u_char>> TWO_UNCOVERED;
extern vector<vector<u_char>> FOUR_COVERED;
extern vector<vector<u_char>> THREE_COVERED;