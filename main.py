''' Main file for Gomoku game '''
import pygame
import random
import time
from gomoku import display, game_setup, ai_api as api
import game_module as gm
import time

STATUS_RUNNING = 0
STATUS_WIN_PLAYER1 = 1
STATUS_WIN_PLAYER2 = 2
STATUS_DRAW = 3
STATUS_TERMINATED = 4
DEPTH = 10

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
    ''' Get the current time in milliseconds '''
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

    def sync_display(board: list[list[int]]) -> None:
        ''' Sync the display with the board '''
        display.draw_board(screen)
        display.draw_pieces(screen, board, hints)
        display.draw_menu(screen, turn, setup["start_rules"], player1, capt[0], player2, capt[1], get_time() - clock)
        pygame.display.flip()

    player1, player2 = parse_player_setup(setup)
    hints_mode = setup["mode"] == game_setup.OPTION_PLAYER_VS_PLAYER_HINTS
    game = gm.Game()
    hints = []
    capt = [0, 0]
    game_status = 0
    turn = 1
    clock = get_time()
    sync_display(game.getBoard())

    while game_status == STATUS_RUNNING:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_status = STATUS_TERMINATED
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                # Player moves
                if (turn == 1 and player1 == "Player") or (turn == 2 and player2 == "Player"):
                    move, inside = display.mouse_click(game.getBoard())
                    if inside:
                        if game.makeMove(turn, move[0], move[1], capt[turn - 1], []):
                            capt[turn - 1] += 1
                        turn = swtich_move(turn)
                        refresh_status(game.getBoard())
                        if hints_mode:
                            hints = api.player_hints(game.getBoard(), turn)
                        sync_display(game.getBoard())
                        clock = get_time()

        if game_status != STATUS_RUNNING:
            break

        # AI moves
        if (turn == 1 and player1 == "AI") or (turn == 2 and player2 == "AI"):
            ai = gm.AI(game, turn)
            _, move = ai.iterativeDeepening(turn, DEPTH)
            if game.makeMove(turn, move[0], move[1], capt[turn - 1], []):
                capt[turn - 1] += 1
            turn = swtich_move(turn)
            refresh_status(game.getBoard())
            sync_display(game.getBoard())
            clock = get_time()

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
