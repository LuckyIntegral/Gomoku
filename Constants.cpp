#include "Constants.hpp"

// WIN
static const int win_arr[5] = {1, 1, 1, 1, 1};
const std::vector<int> WIN(win_arr, win_arr + 5);

// FOUR_UNCOVERED
static const int four_uncovered_arr[6] = {0, 1, 1, 1, 1, 0};
const std::vector<std::vector<int> > FOUR_UNCOVERED(1, std::vector<int>(four_uncovered_arr, four_uncovered_arr + 6));

// THREE_UNCOVERED
static const int three_uncovered_arr0[5] = {0, 1, 1, 1, 0};
static const int three_uncovered_arr1[6] = {0, 1, 1, 0, 1, 0};
static const int three_uncovered_arr2[6] = {0, 1, 0, 1, 1, 0};
std::vector<std::vector<int> > initThree() {
    std::vector<std::vector<int> > v;
    v.push_back(std::vector<int>(three_uncovered_arr0, three_uncovered_arr0+5));
    v.push_back(std::vector<int>(three_uncovered_arr1, three_uncovered_arr1+6));
    v.push_back(std::vector<int>(three_uncovered_arr2, three_uncovered_arr2+6));
    return v;
}
const std::vector<std::vector<int> > THREE_UNCOVERED = initThree();

// TWO_UNCOVERED
static const int two_uncovered_arr0[5] = {0, 1, 1, 0, 0};
static const int two_uncovered_arr1[5] = {0, 0, 1, 1, 0};
static const int two_uncovered_arr2[5] = {0, 1, 0, 1, 0};
static const int two_uncovered_arr3[6] = {0, 1, 0, 0, 1, 0};
std::vector<std::vector<int> > initTwo() {
    std::vector<std::vector<int> > v;
    v.push_back(std::vector<int>(two_uncovered_arr0, two_uncovered_arr0+5));
    v.push_back(std::vector<int>(two_uncovered_arr1, two_uncovered_arr1+5));
    v.push_back(std::vector<int>(two_uncovered_arr2, two_uncovered_arr2+5));
    v.push_back(std::vector<int>(two_uncovered_arr3, two_uncovered_arr3+6));
    return v;
}
const std::vector<std::vector<int> > TWO_UNCOVERED = initTwo();

// FOUR_COVERED
static const int four_covered_arr0[7] = {2, 1, 1, 1, 1, 0, 2};
static const int four_covered_arr1[7] = {2, 1, 1, 1, 0, 1, 2};
static const int four_covered_arr2[7] = {2, 1, 1, 0, 1, 1, 2};
static const int four_covered_arr3[7] = {2, 1, 0, 1, 1, 1, 2};
static const int four_covered_arr4[7] = {2, 0, 1, 1, 1, 1, 2};
std::vector<std::vector<int> > initFourCovered() {
    std::vector<std::vector<int> > v;
    v.push_back(std::vector<int>(four_covered_arr0, four_covered_arr0+7));
    v.push_back(std::vector<int>(four_covered_arr1, four_covered_arr1+7));
    v.push_back(std::vector<int>(four_covered_arr2, four_covered_arr2+7));
    v.push_back(std::vector<int>(four_covered_arr3, four_covered_arr3+7));
    v.push_back(std::vector<int>(four_covered_arr4, four_covered_arr4+7));
    return v;
}
const std::vector<std::vector<int> > FOUR_COVERED = initFourCovered();

// PATTERNS
std::map<std::vector<int>, int> initPatterns() {
    std::map<std::vector<int>, int> m;
    m[WIN] = WIN_WEIGHT;
    m[FOUR_UNCOVERED[0]] = FOUR_UNCOVERED_WEIGHT;
    m[THREE_UNCOVERED[0]] = THREE_UNCOVERED_WEIGHT;
    m[THREE_UNCOVERED[1]] = THREE_UNCOVERED_WEIGHT;
    m[THREE_UNCOVERED[2]] = THREE_UNCOVERED_WEIGHT;
    m[TWO_UNCOVERED[0]] = TWO_UNCOVERED_WEIGHT;
    m[TWO_UNCOVERED[1]] = TWO_UNCOVERED_WEIGHT;
    m[TWO_UNCOVERED[2]] = TWO_UNCOVERED_WEIGHT;
    m[TWO_UNCOVERED[3]] = TWO_UNCOVERED_WEIGHT;
    m[FOUR_COVERED[0]] = FOUR_COVERED_WEIGHT;
    m[FOUR_COVERED[1]] = FOUR_COVERED_WEIGHT;
    m[FOUR_COVERED[2]] = FOUR_COVERED_WEIGHT;
    m[FOUR_COVERED[3]] = FOUR_COVERED_WEIGHT;
    m[FOUR_COVERED[4]] = FOUR_COVERED_WEIGHT;
    return m;
}
const std::map<std::vector<int>, int> PATTERNS = initPatterns();
