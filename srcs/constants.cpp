#include "constants.hpp"

const long WIN_WEIGHT = 1000000000;
const long FOUR_UNCOVERED_WEIGHT = 1000000;
const long THREE_UNCOVERED_WEIGHT = 10000;
const long TWO_UNCOVERED_WEIGHT = 100;
const long FOUR_COVERED_WEIGHT = 9000;
const long THREE_COVERED_WEIGHT = 1000;
const long CAPTURE_WEIGHT = 500000000;

vector<vector<u_char>> WIN = {{'1', '1', '1', '1', '1'}};

vector<vector<u_char>> FOUR_UNCOVERED = {
    {'-', '1', '1', '1', '1', '-'},
};

vector<vector<u_char>> THREE_UNCOVERED = {
    {'-', '1', '1', '1', '-'},
    {'-', '1', '1', '-', '1', '-'},
    {'-', '1', '-', '1', '1', '-'},
};

vector<vector<u_char>> TWO_UNCOVERED = {
    {'-', '1', '1', '-', '-'},
    {'-', '-', '1', '1', '-'},
    {'-', '1', '-', '1', '-'},
    {'-', '1', '-', '-', '1', '-'},
};

vector<vector<u_char>> FOUR_COVERED = {
    {'2', '1', '1', '1', '1', '-', '2'},
    {'2', '1', '1', '1', '-', '1', '2'},
    {'2', '1', '1', '-', '1', '1', '2'},
    {'2', '1', '-', '1', '1', '1', '2'},
    {'2', '-', '1', '1', '1', '1', '2'},
};

vector<vector<u_char>> THREE_COVERED = {
    {'2', '1', '1', '1', '-', '-'},
    {'2', '1', '1', '-', '1', '-'},
    {'2', '1', '-', '1', '1', '-'},
    {'-', '1', '1', '-', '1', '2'},
    {'-', '1', '-', '1', '1', '2'},
    {'-', '-', '1', '1', '1', '2'},
    {'2', '1', '-', '1', '-', '1', '2'},
    {'2', '-', '1', '1', '1', '-', '2'},
    {'2', '1', '1', '-', '-', '1', '2'},
    {'2', '1', '-', '-', '1', '1', '2'}
};