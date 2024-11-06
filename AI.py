import copy
from Constants import (
    PATTERNS,
    PLAYER1,
    PLAYER2,
    EMPTY,
    CAPTURE_WEIGHT,
)
from Game import Game

class AI:
    def __init__(self, game: object, player: int) -> None:
        self._game = copy.deepcopy(game)
        self._player = player
        self._memo = {}
        self._transposition_table = {}

    def _evaluate_board(self) -> int:
        """
        Evaluate the board based on the patterns
        and returns the score
        """
        score = 0
        for pattern, weight in PATTERNS.items():
            score += self._game.count_pattern_on_board(pattern, self._player) * weight
            score -= self._game.count_pattern_on_board(pattern, 3 - self._player) * weight
        return score

    def _hash_board(self) -> str:
        """
        Create a hash of the current board state
        """
        return str(self._game.get_board())

    def minimax(self, player: int, depth: int, alpha: int, beta: int, is_maximizing: bool) -> tuple:
        """
        Minimax algorithm with alpha-beta pruning and memoization
        returns the best score and the best move
        """
        board_hash = self._hash_board()
        transposition_key = (board_hash, depth, is_maximizing)
        if transposition_key in self._transposition_table:
            return self._transposition_table[transposition_key]

        if depth == 0:
            return self._evaluate_board(), None

        best_move = None
        moves = self._game.get_best_possible_moves(player)

        # Move ordering: prioritize moves that capture the most stones
        moves.sort(key=lambda move: self._game.count_captures(player, move[0], move[1]), reverse=True)

        if is_maximizing:
            best_score = -float('inf')
            for move in moves:
                old_game = copy.deepcopy(self._game)
                captures = self._game.make_move(player, move[0], move[1])
                score, _ = self.minimax(3 - player, depth - 1, alpha, beta, False)
                score += captures * CAPTURE_WEIGHT
                self._game = old_game
                if score > best_score:
                    best_score = score
                    best_move = move
                alpha = max(alpha, score)
                if beta <= alpha:
                    break
        else:
            best_score = float('inf')
            for move in moves:
                old_game = copy.deepcopy(self._game)
                captures = self._game.make_move(player, move[0], move[1])
                score, _ = self.minimax(3 - player, depth - 1, alpha, beta, True)
                score -= captures * CAPTURE_WEIGHT
                self._game = old_game
                if score < best_score:
                    best_score = score
                    best_move = move
                beta = min(beta, score)
                if beta <= alpha:
                    break

        self._transposition_table[transposition_key] = (best_score, best_move)
        return best_score, best_move

    def iterative_deepening(self, player: int, max_depth: int) -> tuple:
        """
        Iterative deepening to progressively deepen the search
        """
        best_move = None
        for depth in range(1, max_depth + 1):
            print(f"Depth: {depth}")
            best_score, best_move = self.minimax(player, depth, -float('inf'), float('inf'), True)
        return best_score, best_move