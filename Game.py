import numpy as np
from Constants import (
    EMPTY,
    PLAYER1,
    PLAYER2,
    THREE_UNCOVERED,
    PATTERNS,
    CAPTURE_WEIGHT,
)

class Game:
    def __init__(self) -> None:
        self._board = np.full((19, 19), EMPTY, dtype=int)
        self._player_1_capture = 0
        self._player_2_capture = 0
    
    def get_board(self) -> np.ndarray:
        """
        Returns a copy of the board
        """
        return self._board.copy()
    
    # CHECKERS FOR CAPTURING
    def _is_left_horizontal_capture(self, player: int, row: int, col: int) -> bool:
        if col < 3:
            return False
        return (self._board[row, col-1] == 3 - player and
                self._board[row, col-2] == 3 - player and
                self._board[row, col-3] == player)
    
    def _is_right_horizontal_capture(self, player: int, row: int, col: int) -> bool:
        if col > 15:
            return False
        return (self._board[row, col+1] == 3 - player and
                self._board[row, col+2] == 3 - player and
                self._board[row, col+3] == player)
    
    def _is_up_vertical_capture(self, player: int, row: int, col: int) -> bool:
        if row < 3:
            return False
        return (self._board[row-1, col] == 3 - player and
                self._board[row-2, col] == 3 - player and
                self._board[row-3, col] == player)
    
    def _is_down_vertical_capture(self, player: int, row: int, col: int) -> bool:
        if row > 15:
            return False
        return (self._board[row+1, col] == 3 - player and
                self._board[row+2, col] == 3 - player and
                self._board[row+3, col] == player)
    
    def _is_left_up_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        if row < 3 or col < 3:
            return False
        return (self._board[row-1, col-1] == 3 - player and
                self._board[row-2, col-2] == 3 - player and
                self._board[row-3, col-3] == player)
    
    def _is_right_up_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        if row < 3 or col > 15:
            return False
        return (self._board[row-1, col+1] == 3 - player and
                self._board[row-2, col+2] == 3 - player and
                self._board[row-3, col+3] == player)
    
    def _is_left_down_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        if row > 15 or col < 3:
            return False
        return (self._board[row+1, col-1] == 3 - player and
                self._board[row+2, col-2] == 3 - player and
                self._board[row+3, col-3] == player)
    
    def _is_right_down_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        if row > 15 or col > 15:
            return False
        return (self._board[row+1, col+1] == 3 - player and
                self._board[row+2, col+2] == 3 - player and
                self._board[row+3, col+3] == player)
    
    def _is_capture(self, player: int, row: int, col: int) -> bool:
        horizontal_capture = (
            self._is_left_horizontal_capture(player, row, col) or
            self._is_right_horizontal_capture(player, row, col)
        )
        
        vertical_capture = (
            self._is_up_vertical_capture(player, row, col) or
            self._is_down_vertical_capture(player, row, col)
        )
        
        diagonal_capture = (
            self._is_left_up_diagonal_capture(player, row, col) or
            self._is_right_up_diagonal_capture(player, row, col) or
            self._is_left_down_diagonal_capture(player, row, col) or
            self._is_right_down_diagonal_capture(player, row, col)
        )
        
        return horizontal_capture or vertical_capture or diagonal_capture
    
    def _count_and_remove_captures(self, player: int, row: int, col: int) -> tuple:
        count = 0
        captured_stones = set()
        if self._is_left_horizontal_capture(player, row, col):
            count += 1
            self._board[row, col-1] = EMPTY
            self._board[row, col-2] = EMPTY
            captured_stones.add((row, col-1))
            captured_stones.add((row, col-2))
        if self._is_right_horizontal_capture(player, row, col):
            count += 1
            self._board[row, col+1] = EMPTY
            self._board[row, col+2] = EMPTY
            captured_stones.add((row, col+1))
            captured_stones.add((row, col+2))
        if self._is_up_vertical_capture(player, row, col):
            count += 1
            self._board[row-1, col] = EMPTY
            self._board[row-2, col] = EMPTY
            captured_stones.add((row-1, col))
            captured_stones.add((row-2, col))
        if self._is_down_vertical_capture(player, row, col):
            count += 1
            self._board[row+1, col] = EMPTY
            self._board[row+2, col] = EMPTY
            captured_stones.add((row+1, col))
            captured_stones.add((row+2, col))
        if self._is_left_up_diagonal_capture(player, row, col):
            count += 1
            self._board[row-1, col-1] = EMPTY
            self._board[row-2, col-2] = EMPTY
            captured_stones.add((row-1, col-1))
            captured_stones.add((row-2, col-2))
        if self._is_right_up_diagonal_capture(player, row, col):
            count += 1
            self._board[row-1, col+1] = EMPTY
            self._board[row-2, col+2] = EMPTY
            captured_stones.add((row-1, col+1))
            captured_stones.add((row-2, col+2))
        if self._is_left_down_diagonal_capture(player, row, col):
            count += 1
            self._board[row+1, col-1] = EMPTY
            self._board[row+2, col-2] = EMPTY
            captured_stones.add((row+1, col-1))
            captured_stones.add((row+2, col-2))
        if self._is_right_down_diagonal_capture(player, row, col):
            count += 1
            self._board[row+1, col+1] = EMPTY
            self._board[row+2, col+2] = EMPTY
            captured_stones.add((row+1, col+1))
            captured_stones.add((row+2, col+2))
        return count, captured_stones
    
    def count_captures(self, player: int, row: int, col: int) -> tuple:
        count = 0
        captured_stones = set()
        if self._is_left_horizontal_capture(player, row, col):
            count += 1
            captured_stones.add((row, col-1))
            captured_stones.add((row, col-2))
        if self._is_right_horizontal_capture(player, row, col):
            count += 1
            captured_stones.add((row, col+1))
            captured_stones.add((row, col+2))
        if self._is_up_vertical_capture(player, row, col):
            count += 1
            captured_stones.add((row-1, col))
            captured_stones.add((row-2, col))
        if self._is_down_vertical_capture(player, row, col):
            count += 1
            captured_stones.add((row+1, col))
            captured_stones.add((row+2, col))
        if self._is_left_up_diagonal_capture(player, row, col):
            count += 1
            captured_stones.add((row-1, col-1))
            captured_stones.add((row-2, col-2))
        if self._is_right_up_diagonal_capture(player, row, col):
            count += 1
            captured_stones.add((row-1, col+1))
            captured_stones.add((row-2, col+2))
        if self._is_left_down_diagonal_capture(player, row, col):
            count += 1
            captured_stones.add((row+1, col-1))
            captured_stones.add((row+2, col-2))
        if self._is_right_down_diagonal_capture(player, row, col):
            count += 1
            captured_stones.add((row+1, col+1))
            captured_stones.add((row+2, col+2))
        return count, captured_stones
    
    def _check_pattern_horizontal(self, pattern: list, player: int, row: int, col: int) -> bool:
        for i in range(len(pattern)):
            if col + i >= 19:
                return False
            if self._board[row, col+i] != pattern[i]:
                return False
        return True

    def _check_pattern_vertical(self, pattern: list, player: int, row: int, col: int) -> bool:
        for i in range(len(pattern)):
            if row + i >= 19:
                return False
            if self._board[row+i, col] != pattern[i]:
                return False
        return True
    
    def _check_pattern_diagonal(self, pattern: list, player: int, row: int, col: int) -> bool:
        for i in range(len(pattern)):
            if row + i >= 19 or col + i >= 19:
                return False
            if self._board[row+i, col+i] != pattern[i]:
                return False
        return True
        
    
    def count_pattern_on_board(self, pattern: list, player: int) -> int:
        new_pattern = []
        if player == PLAYER2:
            for cell in pattern:
                if cell == PLAYER1:
                    new_pattern.append(PLAYER2)
                elif cell == PLAYER2:
                    new_pattern.append(PLAYER1)
                else:
                    new_pattern.append(EMPTY)
        else:
            new_pattern = list(pattern)
        count = 0
        for row in range(19):
            for col in range(19):
                count += self._check_pattern_horizontal(new_pattern, player, row, col)
                count += self._check_pattern_vertical(new_pattern, player, row, col)
                count += self._check_pattern_diagonal(new_pattern, player, row, col)
        return count
        
    
    def is_valid_move(self, player: int, row: int, col: int) -> bool:
        if row < 0 or row >= 19 or col < 0 or col >= 19:
            return False
        if self._board[row, col] != EMPTY:
            return False
        if self._is_capture(player, row, col):
            return True
        prev_count = 0
        for array in THREE_UNCOVERED:
            prev_count += self.count_pattern_on_board(array, player)
        self._board[row, col] = player
        new_count = 0
        for array in THREE_UNCOVERED:
            new_count += self.count_pattern_on_board(array, player)
        self._board[row, col] = EMPTY
        return new_count - prev_count < 2
    
    def make_move(self, player: int, row: int, col: int) -> int:
        captures_count, captured_stones = self._count_and_remove_captures(player, row, col)
        self._board[row, col] = player
        if player == PLAYER1:
            self._player_1_capture += captures_count
        else:
            self._player_2_capture += captures_count
        return captures_count, captured_stones
    
    def _evaluate_board(self, player) -> int:
        score = 0
        for pattern, weight in PATTERNS.items():
            score += self.count_pattern_on_board(pattern, player) * weight
            score -= self.count_pattern_on_board(pattern, 3 - player) * weight
        return score
    
    def place_stone(self, player: int, row: int, col: int) -> None:
        self._board[row, col] = player
    
    def undo_the_move(self, player: int, row: int, col: int, captured_stones: set) -> None:
        self._board[row, col] = EMPTY
        for stone in captured_stones:
            self._board[stone[0], stone[1]] = 3 - player
        if player == PLAYER1:
            self._player_1_capture -= len(captured_stones)
        else:
            self._player_2_capture -= len(captured_stones)
    
    def heuristic_evaluation(self, player: int, row: int, col: int) -> int:
        num_of_captures, captured_positions = self.count_captures(player, row, col)
        self._board[row, col] = player
        for r, c in captured_positions:
            self._board[r, c] = EMPTY
        score = self._evaluate_board(player)
        score += num_of_captures * CAPTURE_WEIGHT
        self._board[row, col] = EMPTY
        for r, c in captured_positions:
            self._board[r, c] = 3 - player
        return score
    
    def get_best_possible_moves(self, player: int) -> list:
        moves = set()
        directions = [(-1, -1), (-1, 0), (-1, 1), (0, -1), (0, 1), (1, -1), (1, 0), (1, 1)]

        for row in range(19):
            for col in range(19):
                if self._board[row, col] != EMPTY:
                    for dr, dc in directions:
                        nr, nc = row + dr, col + dc
                        if tuple((nr, nc)) in moves:
                            continue
                        if 0 <= nr < 19 and 0 <= nc < 19 and self.is_valid_move(player, nr, nc):
                            moves.add((nr, nc))
        if not moves:
            moves.add((9, 9))
        return list(moves)