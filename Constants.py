PLAYER1 = 1
PLAYER2 = 2
EMPTY = 0

WIN_WEIGHT = 1000000000
FOUR_UNCOVERED_WEIGHT = 1000000
THREE_UNCOVERED_WEIGHT = 10000
TWO_UNCOVERED_WEIGHT = 100
FOUR_COVERED_WEIGHT = 9000
THREE_COVERED_WEIGHT = 1000
CAPTURE_WEIGHT = 5000000

WIN = (1, 1, 1, 1, 1)

FOUR_UNCOVERED = [
    (0, 1, 1, 1, 1, 0),
]

THREE_UNCOVERED = [
    (0, 1, 1, 1, 0),
    (0, 1, 1, 0, 1, 0),
    (0, 1, 0, 1, 1, 0),
]

TWO_UNCOVERED = [
    (0, 1, 1, 0, 0),
    (0, 0, 1, 1, 0),
    (0, 1, 0, 1, 0),
    (0, 1, 0, 0, 1, 0),
]

FOUR_COVERED = [
    (2, 1, 1, 1, 1, 0, 2),
    (2, 1, 1, 1, 0, 1, 2),
    (2, 1, 1, 0, 1, 1, 2),
    (2, 1, 0, 1, 1, 1, 2),
    (2, 0, 1, 1, 1, 1, 2),
]

THREE_COVERED = [
    (2, 1, 1, 1, 0, 0),
    (2, 1, 1, 0, 1, 0),
    (2, 1, 0, 1, 1, 0),
    (0, 1, 1, 0, 1, 2),
    (0, 1, 0, 1, 1, 2),
    (0, 0, 1, 1, 1, 2),
    (2, 1, 0, 1, 0, 1, 2),
    (2, 0, 1, 1, 1, 0, 2),
    (2, 1, 1, 0, 0, 1, 2),
    (2, 1, 0, 0, 1, 1, 2),
]

PATTERNS = {
    WIN: WIN_WEIGHT,
    FOUR_UNCOVERED[0]: FOUR_UNCOVERED_WEIGHT,
    THREE_UNCOVERED[0]: THREE_UNCOVERED_WEIGHT,
    THREE_UNCOVERED[1]: THREE_UNCOVERED_WEIGHT,
    THREE_UNCOVERED[2]: THREE_UNCOVERED_WEIGHT,
    TWO_UNCOVERED[0]: TWO_UNCOVERED_WEIGHT,
    TWO_UNCOVERED[1]: TWO_UNCOVERED_WEIGHT,
    TWO_UNCOVERED[2]: TWO_UNCOVERED_WEIGHT,
    TWO_UNCOVERED[3]: TWO_UNCOVERED_WEIGHT,
    FOUR_COVERED[0]: FOUR_COVERED_WEIGHT,
    FOUR_COVERED[1]: FOUR_COVERED_WEIGHT,
    FOUR_COVERED[2]: FOUR_COVERED_WEIGHT,
    FOUR_COVERED[3]: FOUR_COVERED_WEIGHT,
    FOUR_COVERED[4]: FOUR_COVERED_WEIGHT,
    THREE_COVERED[0]: THREE_COVERED_WEIGHT,
    THREE_COVERED[1]: THREE_COVERED_WEIGHT,
    THREE_COVERED[2]: THREE_COVERED_WEIGHT,
    THREE_COVERED[3]: THREE_COVERED_WEIGHT,
    THREE_COVERED[4]: THREE_COVERED_WEIGHT,
    THREE_COVERED[5]: THREE_COVERED_WEIGHT,
    THREE_COVERED[6]: THREE_COVERED_WEIGHT,
    THREE_COVERED[7]: THREE_COVERED_WEIGHT,
    THREE_COVERED[8]: THREE_COVERED_WEIGHT,
    THREE_COVERED[9]: THREE_COVERED_WEIGHT,
}