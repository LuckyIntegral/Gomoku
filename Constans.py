PLAYER1 = 1
PLAYER2 = 2
EMPTY = 0

WIN_WEIGHT = 1000000000
FOUR_UNCOVERED_WEIGHT = 1000000
THREE_UNCOVERED_WEIGHT = 10000
TWO_UNCOVERED_WEIGHT = 100
FOUR_COVERED_WEIGHT = 9000
THREE_COVERED_WEIGHT = 1000
CAPTURE_WEIGHT = 500000000

WIN = [[1, 1, 1, 1, 1]]

FOUR_UNCOVERED = [
    [0, 1, 1, 1, 1, 0],
]

THREE_UNCOVERED = [
    [0, 1, 1, 1, 0],
    [0, 1, 1, 0, 1, 0],
    [0, 1, 0, 1, 1, 0],
]

TWO_UNCOVERED = [
    [0, 1, 1, 0, 0],
    [0, 0, 1, 1, 0],
    [0, 1, 0, 1, 0],
    [0, 1, 0, 0, 1],
]

FOUR_COVERED = [
    [2, 1, 1, 1, 1, 0, 2],
    [2, 1, 1, 1, 0, 1, 2],
    [2, 1, 1, 0, 1, 1, 2],
    [2, 1, 0, 1, 1, 1, 2],
    [2, 0, 1, 1, 1, 1, 2],
]

THREE_COVERED = [
    [2, 1, 1, 1, 0, 0],
    [2, 1, 1, 0, 1, 0],
    [2, 1, 0, 1, 1, 0],
    [0, 1, 1, 0, 1, 2],
    [0, 1, 0, 1, 1, 2],
    [0, 0, 1, 1, 1, 2],
    [2, 1, 0, 1, 0, 1, 2],
    [2, 0, 1, 1, 1, 0, 2],
    [2, 1, 1, 0, 0, 1, 2],
    [2, 1, 0, 0, 1, 1, 2],
]