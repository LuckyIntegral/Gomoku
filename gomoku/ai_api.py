''' Game API '''
import game_module as gm


def ai_move(game, turn: int, depth: int, capt) -> tuple[bool, tuple[int, int], int]:
    ''' AI move: returns a tuple of (success, (row, col), depth)'''
    ai = gm.AI(game, turn)
    _, move = ai.iterative_deepening(turn, depth)
    success = game.make_move(turn, move[0], move[1], capt, [])
    last_depth = ai.get_last_depth()
    return success, move, last_depth

def is_draw(board: list[list[int]]) -> bool:
    ''' Check if the game is a draw

    return: True if the board is full
    '''
    return not any(0 in row for row in board)

