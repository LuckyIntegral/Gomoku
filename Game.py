from Constans import (
    EMPTY,
    PLAYER1,
    PLAYER2,
    THREE_UNCOVERED,
)

class Game:
    def __init__(self):
        self._board = [[EMPTY for _ in range(19)] for _ in range(19)]
        self._player_1_capture = 0
        self._player_2_capture = 0
    
    def get_board(self) -> list:
        """
        Returns shallow copy of the board
        """
        return self._board
    
    # CHECKERS FOR CAPTURING
    def _is_left_horizontal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the left horizontal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if col < 3:
            return False
        return (self._board[row][col-1] == 3 - player and
                self._board[row][col-2] == 3 - player and
                self._board[row][col-3] == player)
    
    def _is_right_horizontal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the right horizontal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if col > 15:
            return False
        return (self._board[row][col+1] == 3 - player and
                self._board[row][col+2] == 3 - player and
                self._board[row][col+3] == player)
    
    def _is_up_vertical_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the up vertical capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row < 3:
            return False
        return (self._board[row-1][col] == 3 - player and
                self._board[row-2][col] == 3 - player and
                self._board[row-3][col] == player)
    
    def _is_down_vertical_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the down vertical capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row > 15:
            return False
        return (self._board[row+1][col] == 3 - player and
                self._board[row+2][col] == 3 - player and
                self._board[row+3][col] == player)
    
    def _is_left_up_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the left up diagonal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row < 3 or col < 3:
            return False
        return (self._board[row-1][col-1] == 3 - player and
                self._board[row-2][col-2] == 3 - player and
                self._board[row-3][col-3] == player)
    
    def _is_right_up_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the right up diagonal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row < 3 or col > 15:
            return False
        return (self._board[row-1][col+1] == 3 - player and
                self._board[row-2][col+2] == 3 - player and
                self._board[row-3][col+3] == player)
    
    def _is_left_down_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the left down diagonal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row > 15 or col < 3:
            return False
        return (self._board[row+1][col-1] == 3 - player and
                self._board[row+2][col-2] == 3 - player and
                self._board[row+3][col-3] == player)
    
    def _is_right_down_diagonal_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the right down diagonal capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row > 15 or col > 15:
            return False
        return (self._board[row+1][col+1] == 3 - player and
                self._board[row+2][col+2] == 3 - player and
                self._board[row+3][col+3] == player)
    
    def _is_capture(self, player: int, row: int, col: int) -> bool:
        """
        Check if the capture will happen
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
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
    
    def _count_and_remove_captures(self, player: int, row: int, col: int) -> int:
        """
        Count captures
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        count = 0
        if self._is_left_horizontal_capture(player, row, col):
            count += 1
            self._board[row][col-1] = EMPTY
            self._board[row][col-2] = EMPTY
        if self._is_right_horizontal_capture(player, row, col):
            count += 1
            self._board[row][col+1] = EMPTY
            self._board[row][col+2] = EMPTY
        if self._is_up_vertical_capture(player, row, col):
            count += 1
            self._board[row-1][col] = EMPTY
            self._board[row-2][col] = EMPTY
        if self._is_down_vertical_capture(player, row, col):
            count += 1
            self._board[row+1][col] = EMPTY
            self._board[row+2][col] = EMPTY
        if self._is_left_up_diagonal_capture(player, row, col):
            count += 1
            self._board[row-1][col-1] = EMPTY
            self._board[row-2][col-2] = EMPTY
        if self._is_right_up_diagonal_capture(player, row, col):
            count += 1
            self._board[row-1][col+1] = EMPTY
            self._board[row-2][col+2] = EMPTY
        if self._is_left_down_diagonal_capture(player, row, col):
            count += 1
            self._board[row+1][col-1] = EMPTY
            self._board[row+2][col-2] = EMPTY
        if self._is_right_down_diagonal_capture(player, row, col):
            count += 1
            self._board[row+1][col+1] = EMPTY
            self._board[row+2][col+2] = EMPTY
        return count
    
    def _check_pattern_horizontal(self, pattern: list, player: int, row: int, col: int) -> bool:
        """
        Check if the pattern is in the right horizontal direction
        Takes in the pattern, player (PLAYER1 OR PLAYER2), row and column
        """
        for i in range(len(pattern)):
            if col + i >= 19:
                return False
            if self._board[row][col+i] != pattern[i]:
                return False
        return True

    def _check_pattern_vertical(self, pattern: list, player: int, row: int, col: int) -> bool:
        """
        Check if the pattern is in the down vertical direction
        Takes in the pattern, player (PLAYER1 OR PLAYER2), row and column
        """
        for i in range(len(pattern)):
            if row + i >= 19:
                return False
            if self._board[row+i][col] != pattern[i]:
                return False
        return True
    
    def _check_pattern_diagonal(self, pattern: list, player: int, row: int, col: int) -> bool:
        """
        Check if the pattern is in the right down diagonal direction
        Takes in the pattern, player (PLAYER1 OR PLAYER2), row and column
        """
        for i in range(len(pattern)):
            if row + i >= 19 or col + i >= 19:
                return False
            if self._board[row+i][col+i] != pattern[i]:
                return False
        return True
        
    
    def _count_pattern_on_board(self, pattern: list, player: int) -> int:
        """
        Counts patterns on the board
        Takes in the pattern and the player (PLAYER1 OR PLAYER2)
        """
        player = 3 - player
        count = 0
        for row in range(19):
            for col in range(19):
                count += self._check_pattern_horizontal(pattern, player, row, col)
                count += self._check_pattern_vertical(pattern, player, row, col)
                count += self._check_pattern_diagonal(pattern, player, row, col)
        return count
        
    
    def is_valid_move(self, player: int, row: int, col: int) -> bool:
        """
        Check if the move is valid
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row < 0 or row >= 19 or col < 0 or col >= 19:
            return False
        if self._board[row][col] != EMPTY:
            return False
        if self._is_capture(player, row, col):
            return True
        # Count the number of patterns on the board
        prev_count = 0
        for array in THREE_UNCOVERED:
            prev_count += self._count_pattern_on_board(array, player)
        # Placing the player on the board and count the number of patterns again
        self._board[row][col] = player
        new_count = 0
        for array in THREE_UNCOVERED:
            new_count += self._count_pattern_on_board(array, player)
        self._board[row][col] = EMPTY
        return new_count - prev_count < 2
    
    def make_move(self, player: int, row: int, col: int) -> int:
        """
        Make a move
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        Returns number of captures
        """
        captures = self._count_and_remove_captures(player, row, col)
        self._board[row][col] = player
        if player == PLAYER1:
            self._player_1_capture += captures
        else:
            self._player_2_capture += captures
        return captures
