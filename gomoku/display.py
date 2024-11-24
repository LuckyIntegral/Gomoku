''' all the display functions are here '''
import time
import pygame


BACKGROUND = (50, 50, 50)
BOARD      = (213, 169, 94)
WHITE      = (255, 255, 255)
BLACK      = (0, 0, 0)
GRAY       = (200, 200, 200)
HINT       = (128, 128, 128)
# so far i hate the red color, it is really ugly on gray background, make it more beautiful
RED        = (255, 100, 100)
BLUE       = (30, 144, 255)

WINDOW_SIZE = (1300, 840)
BOARD_SIZE  = 19


def init() -> pygame.Surface:
    ''' Initialize pygame '''
    pygame.init()
    pygame.display.set_caption("Gomoku")
    return pygame.display.set_mode(WINDOW_SIZE)


def draw_board(screen: pygame.Surface) -> None:
    ''' Draw the board '''
    screen.fill(BACKGROUND)
    screen.fill(BOARD, (21, 21, 794, 794))

    for i in range(BOARD_SIZE):
        pygame.draw.line(screen, BLACK, (40, 40 + i * 42), (796, 40 + i * 42), 1)
        pygame.draw.line(screen, BLACK, (40 + i * 42, 40), (40 + i * 42, 796), 1)
    pygame.draw.rect(screen, BLACK, (21, 21, 794, 794), 4)

    for i in range(3, 16, 6):
        for j in range(3, 16, 6):
            pygame.draw.circle(screen, BLACK, (40 + i * 42, 40 + j * 42), 5)


def draw_piece(screen: pygame.Surface, x: int, y: int, color: tuple[int, int, int]) -> None:
    ''' Draw a piece '''
    pygame.draw.circle(screen, color, (40 + x * 42, 40 + y * 42), 20)
    pygame.draw.circle(screen, BLACK, (40 + x * 42, 40 + y * 42), 20, 1)


def draw_pieces(
    screen: pygame.Surface,
    board: list[list[int]],
    hints: list[tuple[int, int]] = None
) -> None:
    ''' Draw all pieces '''
    for y, row in enumerate(board):
        for x, piece in enumerate(row):
            if piece == 1:
                draw_piece(screen, x, y, WHITE)
            elif piece == 2:
                draw_piece(screen, x, y, BLACK)

    if hints:
        for x, y in hints:
            draw_piece(screen, x, y, HINT)


def mouse_click(board: list[list[int]]) -> tuple[tuple[int, int], bool]:
    ''' Get the mouse click '''
    x, y = pygame.mouse.get_pos()
    x = int((x - 40) / 42 + 0.5)
    y = int((y - 40) / 42 + 0.5)
    if 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE:
        if board[y][x] == 0:
            return (y, x), True
    return (0, 0), False


def request_window(screen: pygame.Surface, options: list[str]) -> int:
    ''' Prompts user to select his color once again '''
    font = pygame.font.Font(None, 36)
    texts = [font.render(option, True, WHITE) for option in options]
    text_rects = [text.get_rect(center=(1050, 130 + 40 * i)) for i, text in enumerate(texts)]

    while True:
        screen.fill(BACKGROUND, (920, 100, 260, 50 * len(options)))
        for text, text_rect in zip(texts, text_rects):
            screen.blit(text, text_rect)
        pygame.draw.rect(screen, GRAY, (920, 100, 260, 50 * len(options)), 2)
        pygame.display.flip()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit(0)
            if event.type == pygame.MOUSEBUTTONDOWN:
                x, y = pygame.mouse.get_pos()
                if 920 <= x <= 1180 and 100 <= y <= (100 + 50 * len(options)):
                    if y <= 150:
                        return 0
                    elif y <= 200:
                        return 1
                    else:
                        return 2
        pygame.time.wait(10)


def draw_menu(
    screen: pygame.Surface,
    turn: int,
    start_rule: str,
    player1_name: str,
    player1_captured: int,
    player2_name: str,
    player2_captured: int,
    total_time: int,
) -> None:
    ''' Draw the menu '''

    big_font = pygame.font.Font(None, 52)
    big_font.set_script("lati")
    font = pygame.font.Font(None, 36)
    small_font = pygame.font.Font(None, 20)

    # Display the start rule and its description
    text = font.render("Start Rule:", True, WHITE)
    screen.blit(text, (850, 40))
    text = big_font.render(start_rule, True, WHITE)
    screen.blit(text, (990, 33))

    # Display the current turn
    text = font.render(f"Player {turn}'s turn", True, WHITE)
    screen.blit(text, (850, 220))

    # Display player1 information
    pygame.draw.circle(screen, WHITE, (1200, 330), 40)
    pygame.draw.rect(screen, GRAY, (840, 265, 430, 135), 2)
    text = font.render("Player 1:", True, WHITE)
    screen.blit(text, (850, 280))
    text = font.render(player1_name, True, RED)
    screen.blit(text, (960, 280))
    text = font.render(f"Captured:", True, WHITE)
    screen.blit(text, (850, 320))
    text = font.render(f"{player1_captured} / 5", True, RED)
    screen.blit(text, (850, 360))

    # Display player2 information
    pygame.draw.circle(screen, BLACK, (1200, 470), 40)
    pygame.draw.rect(screen, GRAY, (840, 405, 430, 135), 2)
    text = font.render("Player 2:", True, WHITE)
    screen.blit(text, (850, 420))
    text = font.render(player2_name, True, BLUE)
    screen.blit(text, (960, 420))
    text = font.render(f"Captured:", True, WHITE)
    screen.blit(text, (850, 460))
    text = font.render(f"{player2_captured} / 5", True, BLUE)
    screen.blit(text, (850, 500))

    text = font.render(f"Time spent on the last move: {total_time // 6000}:{total_time // 100 % 60:02}:{total_time % 100:02}", True, WHITE)
    screen.blit(text, (850, 560))

    # Display the rules of Gomoku
    text = small_font.render("Rules: ", True, GRAY)
    screen.blit(text, (830, 680))
    text = small_font.render("Align 5 or more stones to win.", True, GRAY)
    screen.blit(text, (830, 700))
    text = small_font.render("Board size: 19x19.", True, GRAY)
    screen.blit(text, (830, 720))
    text = small_font.render("Capture pairs to win by capturing 5 stones.", True, GRAY)
    screen.blit(text, (830, 740))
    text = small_font.render("Endgame: Win if opponent can't capture a pair.", True, GRAY)
    screen.blit(text, (830, 760))
    text = small_font.render("Lose if opponent captures 5 pairs.", True, GRAY)
    screen.blit(text, (830, 780))
    text = small_font.render("No double-threes allowed.", True, GRAY)
    screen.blit(text, (830, 800))
