import copy
from Constants import (
    PATTERNS,
    PLAYER1,
    PLAYER2,
    EMPTY,
    CAPTURE_WEIGHT,
)
from Game import Game
import time

class AI:
    def __init__(self, game: Game, player: int) -> None:
        self._game = game
        self._player = player
        self._transposition_table = {}

    def _evaluate_board(self) -> int:
        start_time = time.time()
        score = 0
        for pattern, weight in PATTERNS.items():
            score += self._game.count_pattern_on_board(pattern, self._player) * weight
            score -= self._game.count_pattern_on_board(pattern, 3 - self._player) * weight
        end_time = time.time()
        print(f"_evaluate_board took {end_time - start_time:.6f} seconds")
        return score

    def _hash_board(self) -> str:
        start_time = time.time()
        board_hash = str(self._game.get_board())
        end_time = time.time()
        print(f"_hash_board took {end_time - start_time:.6f} seconds")
        return board_hash

    def minimax(self, player: int, depth: int, alpha: float, beta: float, is_maximizing: bool) -> tuple:
        start_time = time.time()
        board_hash = self._hash_board()
        transposition_key = (board_hash, depth, is_maximizing)
        if transposition_key in self._transposition_table:
            end_time = time.time()
            print(f"minimax (transposition table hit) took {end_time - start_time:.6f} seconds")
            return self._transposition_table[transposition_key]

        if depth == 0:
            end_time = time.time()
            print(f"minimax (depth 0) took {end_time - start_time:.6f} seconds")
            return self._evaluate_board(), None

        best_move = None
        moves = self._game.get_best_possible_moves(player)

        # Move ordering: prioritize moves based on a heuristic
        moves.sort(key=lambda move: self._game.heuristic_evaluation(player, move[0], move[1]), reverse=True)

        if is_maximizing:
            best_score = -float('inf')
            for move in moves[:2]:
                captures_count, captured_stones = self._game.make_move(player, move[0], move[1])
                score, _ = self.minimax(3 - player, depth - 1, alpha, beta, False)
                score += captures_count * CAPTURE_WEIGHT
                self._game.undo_the_move(player, move[0], move[1], captured_stones)  # Properly revert the move

                if score > best_score:
                    best_score = score
                    best_move = move
                alpha = max(alpha, score)
                if beta <= alpha:
                    break
        else:
            best_score = float('inf')
            for move in moves[:2]:
                captures_count, captured_stones = self._game.make_move(player, move[0], move[1])
                score, _ = self.minimax(3 - player, depth - 1, alpha, beta, True)
                score += captures_count * CAPTURE_WEIGHT
                self._game.undo_the_move(player, move[0], move[1], captured_stones)  # Properly revert the move

                if score < best_score:
                    best_score = score
                    best_move = move
                beta = min(beta, score)
                if beta <= alpha:
                    break

        self._transposition_table[transposition_key] = (best_score, best_move)
        end_time = time.time()
        print(f"minimax took {end_time - start_time:.6f} seconds")
        return best_score, best_move

    def iterative_deepening(self, player: int, max_depth: int) -> tuple:
        best_move = None
        print(f"Depth: {max_depth}")
        start_time = time.time()
        best_score, best_move = self.minimax(player, max_depth, -float('inf'), float('inf'), True)
        end_time = time.time()
        print(f"iterative_deepening (depth {max_depth}) took {end_time - start_time:.6f} seconds")
        return best_score, best_move