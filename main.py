''' Main file for Gomoku game '''
import pygame
import random
from gomoku import display, game_setup, api


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

    if setup["color"] == game_setup.OPTION_WHITE:
        return "Player", "Player"
    return "Player", "Player"


def main():
    ''' Main function for Gomoku game '''
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
    screen: pygame.Surface = display.init()
    setup = game_setup.prompt_game_setup(screen)
    player1, player2 = parse_player_setup(setup)
    board = [[0 for _ in range(19)] for _ in range(19)]
    game_status = 0
    turn = 1

    while game_status == STATUS_RUNNING:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                game_status = STATUS_TERMINATED
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                # Player moves
                if turn == 1 and player1 == "Player":
                    if display.mouse_click(board, turn):
                        turn = 2
                        refresh_status(board)
                elif turn == 2 and player2 == "Player":
                    if display.mouse_click(board, turn):
                        turn = 1
                        refresh_status(board)

        if game_status != STATUS_RUNNING:
            break

        # AI moves
        if turn == 1 and player1 == "AI":
            x, y = api.ai_move(board, 1)
            board[y][x] = 1
            turn = 2
            refresh_status(board)
        elif turn == 2 and player2 == "AI":
            x, y = api.ai_move(board, 2)
            board[y][x] = 2
            turn = 1
            refresh_status(board)

        display.draw_board(screen)
        display.draw_pieces(screen, board)
        display.draw_menu(screen, turn)

        pygame.display.flip()
        pygame.time.delay(10)

    print("Game Over")
    print(f"Exit status {game_status}")


if __name__ == "__main__":
    main()
