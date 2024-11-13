#pragma once

#include <vector>
#include <map>
#include <array>

const int PLAYER1 = 1;
const int PLAYER2 = 2;
const int EMPTY = 0;

const int WIN_WEIGHT = 10000000;
const int FOUR_UNCOVERED_WEIGHT = 1000;
const int THREE_UNCOVERED_WEIGHT = 600;
const int TWO_UNCOVERED_WEIGHT = 1;
const int FOUR_COVERED_WEIGHT = 90;
const int THREE_COVERED_WEIGHT = 10;
const int CAPTURE_WEIGHT = 50000;

const std::vector<int> WIN = {1, 1, 1, 1, 1};

const std::vector<std::vector<int>> FOUR_UNCOVERED = {
    {0, 1, 1, 1, 1, 0},
};

const std::vector<std::vector<int>> THREE_UNCOVERED = {
    {0, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 0},
    {0, 1, 0, 1, 1, 0},
};

const std::vector<std::vector<int>> TWO_UNCOVERED = {
    {0, 1, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 0, 1, 0},
};

const std::vector<std::vector<int>> FOUR_COVERED = {
    {2, 1, 1, 1, 1, 0, 2},
    {2, 1, 1, 1, 0, 1, 2},
    {2, 1, 1, 0, 1, 1, 2},
    {2, 1, 0, 1, 1, 1, 2},
    {2, 0, 1, 1, 1, 1, 2},
};

// const std::vector<std::vector<int>> THREE_COVERED = {
//     {2, 1, 1, 1, 0, 0},
//     {2, 1, 1, 0, 1, 0},
//     {2, 1, 0, 1, 1, 0},
//     {0, 1, 1, 0, 1, 2},
//     {0, 1, 0, 1, 1, 2},
//     {0, 0, 1, 1, 1, 2},
//     {2, 1, 0, 1, 0, 1, 2},
//     {2, 0, 1, 1, 1, 0, 2},
//     {2, 1, 1, 0, 0, 1, 2},
//     {2, 1, 0, 0, 1, 1, 2},
// };

const std::map<std::vector<int>, int> PATTERNS = {
    {WIN, WIN_WEIGHT},
    {FOUR_UNCOVERED[0], FOUR_UNCOVERED_WEIGHT},
    {THREE_UNCOVERED[0], THREE_UNCOVERED_WEIGHT},
    {THREE_UNCOVERED[1], THREE_UNCOVERED_WEIGHT},
    {THREE_UNCOVERED[2], THREE_UNCOVERED_WEIGHT},
    {TWO_UNCOVERED[0], TWO_UNCOVERED_WEIGHT},
    {TWO_UNCOVERED[1], TWO_UNCOVERED_WEIGHT},
    {TWO_UNCOVERED[2], TWO_UNCOVERED_WEIGHT},
    {TWO_UNCOVERED[3], TWO_UNCOVERED_WEIGHT},
    {FOUR_COVERED[0], FOUR_COVERED_WEIGHT},
    {FOUR_COVERED[1], FOUR_COVERED_WEIGHT},
    {FOUR_COVERED[2], FOUR_COVERED_WEIGHT},
    {FOUR_COVERED[3], FOUR_COVERED_WEIGHT},
    {FOUR_COVERED[4], FOUR_COVERED_WEIGHT},
    // {THREE_COVERED[0], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[1], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[2], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[3], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[4], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[5], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[6], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[7], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[8], THREE_COVERED_WEIGHT},
    // {THREE_COVERED[9], THREE_COVERED_WEIGHT},
};
