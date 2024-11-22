''' This module contains the game setup prompt. '''
import sys
import pygame
from . import display

OPTION_PLAYER_VS_AI = "Player vs AI"
OPTION_PLAYER_VS_PLAYER = "Player vs Player"
OPTION_PLAYER_VS_PLAYER_HINTS = "Player vs Player with Hints"
OPTION_WHITE = "White"
OPTION_BLACK = "Black"
OPTION_RANDOM = "Random"
OPTION_STANDARD = "Standard"
OPTION_SWAP = "Swap"
OPTION_SWAP2 = "Swap2"
OPTION_PRO = "Pro"
OPTION_LONG_PRO = "Long Pro"

PLAYER_MODE_OPTIONS = [
    OPTION_PLAYER_VS_AI,
    OPTION_PLAYER_VS_PLAYER,
    OPTION_PLAYER_VS_PLAYER_HINTS
]
PLAYER_COLOR_OPTIONS = [
    OPTION_WHITE,
    OPTION_BLACK,
    OPTION_RANDOM
]
RULES_OPTIONS = [
    OPTION_STANDARD,
    OPTION_SWAP,
    OPTION_SWAP2,
    OPTION_PRO,
    OPTION_LONG_PRO
]

MENU_X_POS = 850

def create_selector_button(
    screen: pygame.Surface,
    text: str,
    position: tuple[int, int],
    font: pygame.font.Font,
    enabled: bool = True
) -> pygame.Rect:
    ''' Draws a button on the screen with the given text. '''
    if enabled:
        text_surface = font.render(text, True, (255, 255, 255))
        width = 3
    else:
        text_surface = font.render(text, True, (100, 100, 100))
        width = 0
    text_rect = text_surface.get_rect(topleft=position)
    button_rect = text_rect.inflate(20, 10)
    pygame.draw.rect(
        screen,
        (200, 200, 200),
        button_rect,
        width,
        border_radius=15,
    )
    screen.blit(text_surface, text_rect)
    return button_rect


def make_static_menu_entry(
    text: str,
    position: tuple[int, int],
    header: bool = False
) -> tuple[pygame.Surface, pygame.Rect]:
    ''' Creates a text placeholder for the menu. This text is not interactive. '''
    font_size = 36 if header else 32

    player_mode_text = pygame.font.Font("freesansbold.ttf", font_size)\
        .render(text, True, (255, 255, 255))
    player_mode_rect = player_mode_text.get_rect(topleft=position)

    return player_mode_text, player_mode_rect


def add_buttons(
    screen: pygame.Surface,
    options: list[str],
    position: tuple[int, int],
    selected: int
) -> list[pygame.Rect]:
    ''' Draws buttons for each option and returns a list of rects for each button. '''
    BUTTON_FONT = pygame.font.Font("freesansbold.ttf", 24)
    BUTTON_FONT.italic = True
    reacts = []

    for i, option in enumerate(options):
        reacts.append(create_selector_button(
            screen,
            option,
            position,
            BUTTON_FONT,
            i == selected
        ))
        position = (position[0], position[1] + 50)

    return reacts


def listen_clicks(
    reacts: list[pygame.Rect],
    x: int,
    y: int,
    old_mode: int
) -> int:
    ''' Listens for clicks on the buttons and returns the new mode if clicked. '''

    for i, button_rect in enumerate(reacts):
        if button_rect.collidepoint(x, y):
            return i

    return old_mode


def create_start_button(screen: pygame.Surface) -> pygame.Rect:
    ''' Creates a start button. '''
    font = pygame.font.Font("freesansbold.ttf", 32)
    text_surface = font.render("Start game", True, (30, 215, 30))
    text_rect = text_surface.get_rect(topright=(MENU_X_POS + 400, 750))
    button_rect = text_rect.inflate(20, 10)
    pygame.draw.rect(screen, (10, 160, 10), button_rect, 3)
    screen.blit(text_surface, text_rect)
    return button_rect


def frames(freq: int = 3):
    ''' Generator for the animation of the pieces. '''
    board = [
        [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0],
        [0,0,0,0,0,2,2,1,1,1,1,1,2,2,0,0,0,0,0],
        [0,0,0,0,2,1,1,1,1,1,1,1,1,2,2,0,0,0,0],
        [0,0,0,2,1,1,1,2,2,1,1,1,1,2,2,2,0,0,0],
        [0,0,0,2,1,1,1,2,2,1,1,1,1,2,2,2,0,0,0],
        [0,0,2,1,1,1,1,1,1,1,1,1,1,2,2,2,2,0,0],
        [0,0,2,1,1,1,1,1,1,1,1,1,2,2,2,2,2,0,0],
        [0,0,2,1,1,1,1,1,1,1,1,2,2,2,2,2,2,0,0],
        [0,0,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,0,0],
        [0,0,2,1,1,1,1,1,2,2,1,1,2,2,2,2,2,0,0],
        [0,0,0,2,1,1,1,2,2,2,1,1,2,2,2,2,0,0,0],
        [0,0,0,2,2,1,1,2,2,2,2,2,2,2,2,2,0,0,0],
        [0,0,0,0,2,1,1,1,2,2,2,2,2,2,2,0,0,0,0],
        [0,0,0,0,0,2,2,1,1,2,2,2,2,2,0,0,0,0,0],
        [0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
        [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    ]
    while True:
        yield board


def convert_to_text(settings: dict) -> dict:
    ''' Converts the settings to text. '''
    settings["mode"] = PLAYER_MODE_OPTIONS[settings["mode"]]
    settings["color"] = PLAYER_COLOR_OPTIONS[settings["color"]]
    settings["start_rules"] = RULES_OPTIONS[settings["start_rules"]]
    return settings


def prompt_game_setup(screen: pygame.Surface) -> dict:
    ''' Prompts the player to choose starting stats and position. '''
    settings = { # Default settings
        "mode": 0,
        "color": 2,
        "start_rules": 0
    }
    STATIC_MENU_TEXT = [
        make_static_menu_entry("Settings", (MENU_X_POS + 120, 20), header=True),
        make_static_menu_entry("Player Mode:", (MENU_X_POS, 90)),
        make_static_menu_entry("Color to play:", (MENU_X_POS, 290)),
        make_static_menu_entry("Starting Position:", (MENU_X_POS, 490))
    ]


    for board in frames():
        display.draw_board(screen)
        display.draw_pieces(screen, board)

        for text, rect in STATIC_MENU_TEXT:
            screen.blit(text, rect)

        mode_reacts = add_buttons(screen, PLAYER_MODE_OPTIONS, (MENU_X_POS, 140), settings["mode"])
        color_reacts = add_buttons(screen, PLAYER_COLOR_OPTIONS, (MENU_X_POS, 340), settings["color"])
        rules_reacts = add_buttons(screen, RULES_OPTIONS, (MENU_X_POS, 540), settings["start_rules"])
        start_rect = create_start_button(screen)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                x, y = pygame.mouse.get_pos()
                settings["mode"] = listen_clicks(mode_reacts, x, y, settings["mode"])
                settings["color"] = listen_clicks(color_reacts, x, y, settings["color"])
                settings["start_rules"] = listen_clicks(rules_reacts, x, y, settings["start_rules"])
                if start_rect.collidepoint(x, y):
                    return convert_to_text(settings)

        pygame.display.flip()
        pygame.time.delay(50)

    return convert_to_text(settings)
