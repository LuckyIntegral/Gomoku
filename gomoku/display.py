''' all the display functions are here '''
import pygame

BACKGROUND = (50, 50, 50)
BOARD      = (213, 169, 94)
WHITE      = (255, 255, 255)
BLACK      = (0, 0, 0)

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


def draw_pieces(screen: pygame.Surface, board: list[list[int]]) -> None:
    ''' Draw all pieces '''
    for y, row in enumerate(board):
        for x, piece in enumerate(row):
            if piece == 1:
                draw_piece(screen, x, y, WHITE)
            elif piece == 2:
                draw_piece(screen, x, y, BLACK)


def mouse_click(board: list[list[int]], player: int) -> bool:
    ''' Get the mouse click '''
    x, y = pygame.mouse.get_pos()
    x = int((x - 40) / 42 + 0.5)
    y = int((y - 40) / 42 + 0.5)
    if 0 <= x < BOARD_SIZE and 0 <= y < BOARD_SIZE:
        if board[y][x] == 0:
            board[y][x] = player
            return True
    return False


def draw_menu(screen: pygame.Surface, turn: int) -> None:
    ''' Draw the menu '''
    font = pygame.font.Font(None, 36)
    text = font.render(f"Player {turn}'s turn", True, WHITE)
    screen.blit(text, (850, 40))
    text = font.render("Press 'Q' to quit", True, WHITE)
    screen.blit(text, (850, 80))
    text = font.render("Press 'R' to restart", True, WHITE)
    screen.blit(text, (850, 120))
    text = font.render("Press 'H' to show help", True, WHITE)
    screen.blit(text, (850, 160))
