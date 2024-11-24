''' Game API '''
import game_module as gm
import random


def ai_move(game, turn: int, depth: int, capt) -> bool:
    ''' AI move

    return: Number of captures
    '''
    ai = gm.AI(game, turn)
    _, move = ai.iterativeDeepening(turn, depth)
    print(f"AI move: {move}")
    return game.makeMove(turn, move[0], move[1], capt, [])

def is_draw(board: list[list[int]]) -> bool:
    ''' Check if the game is a draw

    return: True if the board is full
    '''
    return not any(0 in row for row in board)

def is_win(board: list[list[int]], player: int) -> bool:
    ''' check if the player has won '''
    # api call here
    # if random.randint(0, 100) == 42:
    #     return True
    return False
