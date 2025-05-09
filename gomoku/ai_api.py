''' Game API '''
import game_module as gm


def ai_move(game, turn: int, depth: int, capt) -> tuple[bool, tuple[int, int]]:
    ''' AI move

    return: (success, (row, col))
    '''
    ai = gm.AI(game, turn)
    _, move = ai.iterative_deepening(turn, depth)
    success = game.make_move(turn, move[0], move[1], capt, [])
    return success, move

def is_draw(board: list[list[int]]) -> bool:
    ''' Check if the game is a draw

    return: True if the board is full
    '''
    return not any(0 in row for row in board)

# def is_win(board: list[list[int]], player: int) -> bool:
#     ''' checks if the player has won the game '''
#     directions = [(0, 1), (1, 0), (1, 1), (1, -1)]

#     for row in range(19):
#         for col in range(19):
#             if board[row][col] == player:
#                 for d in directions:
#                     if row + 4 * d[0] >= 19:
#                         continue
#                     if col + 4 * d[1] < 0 or col + 4 * d[1] >= 19:
#                         continue
#                     for i in range(1, 5):
#                         if board[row + i * d[0]][col + i * d[1]] != player:
#                             if i == 5:
#                                 print(f"Player {player} wins!")
#                             break
#                     else:
#                         return True
#     return False
