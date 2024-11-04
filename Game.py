PLAYER1 = 1
PLAYER2 = 2
EMPTY = 0

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
    
    def is_valid_move(self, player: int, row: int, col: int) -> bool:
        """
        Check if the move is valid
        Takes in the player (PLAYER1 OR PLAYER2), row and column
        """
        if row < 0 or row >= 19 or col < 0 or col >= 19:
            return False
        if self._board[row][col] != EMPTY:
            return False
    
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
    
    