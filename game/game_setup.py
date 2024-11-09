''' This module contains the game setup prompt. '''
import sys
import pygame
from . import display


def draw_button(screen, text, position, font, color=(0, 0, 0), bg_color=(200, 200, 200)):
    text_surface = font.render(text, True, color)
    text_rect = text_surface.get_rect(center=position)
    button_rect = text_rect.inflate(20, 10)
    pygame.draw.rect(screen, bg_color, button_rect)
    screen.blit(text_surface, text_rect)
    return button_rect


def prompt_game_setup(screen: pygame.Surface) -> dict:
    ''' Prompts the player to choose starting stats and position. '''
    mode = {
        "player_mode": 1,
        "player_color": 1,
        "starting_position": 1
    }
    running = True
    font = pygame.font.Font(None, 36)

    # Menu options
    player_mode_options = ["Player vs AI", "Player vs Player", "Player vs Player with Hints"]
    player_color_options = ["White", "Black"]
    starting_position_options = ["Swap", "Swap2", "Pro", "Long Pro"]

    player_mode_text = font.render("Player Mode:", True, (0, 0, 0))
    player_color_text = font.render("Player Color:", True, (0, 0, 0))
    starting_position_text = font.render("Starting Position:", True, (0, 0, 0))

    player_mode_rect = player_mode_text.get_rect(topleft=(50, 100))
    player_color_rect = player_color_text.get_rect(topleft=(50, 200))
    starting_position_rect = starting_position_text.get_rect(topleft=(50, 300))

    while running:
        display.draw_board(screen)
        screen.blit(player_mode_text, player_mode_rect)
        screen.blit(player_color_text, player_color_rect)
        screen.blit(starting_position_text, starting_position_rect)

        player_mode_button_rect = draw_button(screen, player_mode_options[mode["player_mode"] - 1], (200, 100), font)
        player_color_button_rect = draw_button(screen, player_color_options[mode["player_color"] - 1], (200, 200), font)
        starting_position_button_rect = draw_button(screen, starting_position_options[mode["starting_position"] - 1], (200, 300), font)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                x, y = pygame.mouse.get_pos()
                if player_mode_button_rect.collidepoint(x, y):
                    mode["player_mode"] = (mode["player_mode"] % 3) + 1
                elif player_color_button_rect.collidepoint(x, y):
                    mode["player_color"] = (mode["player_color"] % 2) + 1
                elif starting_position_button_rect.collidepoint(x, y):
                    mode["starting_position"] = (mode["starting_position"] % 4) + 1

        pygame.display.flip()
        pygame.time.delay(10)

    return mode
