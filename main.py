''' Main file for Gomoku game '''
import pygame
import random
import time
from gomoku import display, game_setup, ai_api as api


STATUS_RUNNING = 0
STATUS_WIN_PLAYER1 = 1
STATUS_WIN_PLAYER2 = 2
STATUS_DRAW = 3
STATUS_TERMINATED = 4

def parse_player_setup(setup: dict[str, int]) -> tuple[str, str]:
    ''' Parse the player setup '''
    if setup["color"] == game_setup.OPTION_RANDOM:
        setup["color"] = random.choice([game_setup.OPTION_WHITE, game_setup.OPTION_BLACK])

    if setup["mode"] == game_setup.OPTION_PLAYER_VS_AI:
        if setup["color"] == game_setup.OPTION_WHITE:
            return "Player", "AI"
        return "AI", "Player"

    return "Player", "Player"


def swtich_move(turn: int) -> int:
    ''' Switch the turn
    1 -> 2
    2 -> 1
    '''
    return 3 - turn


def get_time() -> int:
    ''' Get the current time in seconds '''
    return time.time_ns() // 10000000



def game_loop(screen: pygame.Surface, setup: dict[str, int]) -> int:
    ''' Main game loop '''
    def refresh_status(board: list[list[int]]) -> None:
        ''' Refresh the game status '''
        nonlocal game_status
        if api.is_win(board, 1):
            game_status = STATUS_WIN_PLAYER1
        elif api.is_win(board, 2):
            game_status = STATUS_WIN_PLAYER2
        elif api.is_draw(board):
            game_status = STATUS_DRAW
        else:
            game_status = STATUS_RUNNING

    player1, player2 = parse_player_setup(setup)
    time1, time2 = 0, 0
    board = [[0 for _ in range(19)] for _ in range(19)]
    game_status = 0
    turn = 1
    clock = get_time()

    while game_status == STATUS_RUNNING:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_status = STATUS_TERMINATED
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                # Player moves
                if (turn == 1 and player1 == "Player") or (turn == 2 and player2 == "Player"):
                    if display.mouse_click(board, turn):
                        turn = swtich_move(turn)
                        refresh_status(board)

        if game_status != STATUS_RUNNING:
            break

        # AI moves
        if (turn == 1 and player1 == "AI") or (turn == 2 and player2 == "AI"):
            x, y = api.ai_move(board, turn)
            board[y][x] = turn
            turn = swtich_move(turn)
            refresh_status(board)

        if turn == 1:
            time1 += get_time() - clock
        else:
            time2 += get_time() - clock
        clock = get_time()

        display.draw_board(screen)
        display.draw_pieces(screen, board)
        display.draw_menu(screen, turn, setup["start_rules"], player1, 3, time1, player2, 4, time2, time1 + time2)

        pygame.display.flip()
        pygame.time.delay(100)


    return game_status


def main():
    ''' Main function for Gomoku game '''
    screen: pygame.Surface = display.init()

    setup = game_setup.prompt_game_setup(screen)
    game_status = game_loop(screen, setup)

    print("Game Over")
    print(f"Exit status {game_status}")


if __name__ == "__main__":
    main()
