''' Main file for Gomoku game '''
import pygame
from gomoku import display, game_setup, game as gm


def main():
    ''' Main function for Gomoku game '''
    screen: pygame.Surface = display.init()

    setup = game_setup.prompt_game_setup(screen)
    game = gm.Game(screen, setup)
    game.loop()


if __name__ == "__main__":
    main()
