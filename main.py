''' Main file for Gomoku game '''
import pygame
from game import display, game_setup


def main():
    ''' Main function for Gomoku game '''
    screen: pygame.Surface = display.init()
    setup = game_setup.prompt_game_setup(screen)
    board = [[0 for _ in range(19)] for _ in range(19)]
    running = True
    turn = 1

    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                if display.mouse_click(board, turn):
                    turn = turn % 2 + 1

        display.draw_board(screen)
        display.draw_pieces(screen, board)
        display.draw_menu(screen, turn)

        pygame.display.flip()
        pygame.time.delay(10)

    pygame.quit()


if __name__ == "__main__":
    main()
