''' Main game module '''
import pygame
import random
import time
from . import display, game_setup, ai_api as api
import game_module as gm
import time

STATUS_RUNNING = 0
STATUS_WIN_PLAYER1 = 1
STATUS_WIN_PLAYER2 = 2
STATUS_DRAW = 3
STATUS_TERMINATED = 4
DEPTH = 5


class Game:
    ''' Main game class '''
    def __init__(self, screen: pygame.Surface, setup: dict[str, int]):
        ''' Initialize the game '''
        self.board = [[0 for _ in range(19)] for _ in range(19)]
        self.player1, self.player2 = self.parse_player_setup(setup)
        self.hints_mode = setup["mode"] == game_setup.OPTION_PLAYER_VS_PLAYER_HINTS
        self.captures = [0, 0]
        self.capt = [0, 0]
        self.hints = []
        self.turn = 1

        self.game_status = 0
        self.game = gm.Game()
        self.screen = screen
        self.setup = setup
        self.clock = self.get_time()


    def parse_player_setup(self, setup: dict[str, int]) -> tuple[str, str]:
        ''' Parse the player setup '''
        if setup["color"] == game_setup.OPTION_RANDOM:
            setup["color"] = random.choice([game_setup.OPTION_WHITE, game_setup.OPTION_BLACK])

        if setup["mode"] == game_setup.OPTION_PLAYER_VS_AI:
            if setup["color"] == game_setup.OPTION_WHITE:
                return "Player", "AI"
            return "AI", "Player"

        return "Player", "Player"


    def get_time(self) -> int:
        ''' Get the current time in milliseconds '''
        return time.time_ns() // 10000000


    def swtich_move(self) -> None:
        ''' Switch the  turn
        1 -> 2
        2 -> 1
        '''
        self.turn = 3 - self.turn


    def refresh_status(self) -> None:
        ''' Refresh the game status '''
        if api.is_win(self.game.getBoard(), 1):
            self.game_status = STATUS_WIN_PLAYER1
        elif api.is_win(self.game.getBoard(), 2):
            self.game_status = STATUS_WIN_PLAYER2
        elif api.is_draw(self.game.getBoard()):
            self.game_status = STATUS_DRAW
        else:
            self.game_status = STATUS_RUNNING


    def sync_display(self) -> None:
        ''' Sync the display with the board '''
        display.draw_board(self.screen)
        display.draw_pieces(self.screen, self.game.getBoard(), self.hints)
        display.draw_menu(self.screen, self.turn, self.setup["start_rules"], self.player1, self.capt[0], self.player2, self.capt[1], self.get_time() - self.clock)
        pygame.display.flip()


    def aftermove(self):
        ''' After move '''
        self.swtich_move()
        self.refresh_status()
        self.sync_display()
        self.capt[self.turn - 1] += self.game.getCaptures(self.turn)
        self.clock = self.get_time()


    def loop(self) -> int:
        ''' Main game loop '''
        self.sync_display()

        while self.game_status == STATUS_RUNNING:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    self.game_status = STATUS_TERMINATED
                if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                    # Player moves
                    if (self.turn == 1 and self.player1 == "Player")\
                            or (self.turn == 2 and self.player2 == "Player"):
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            # if hints_mode:
                            #     hints = api.player_hints(game.getBoard(), self.turn)
                            self.aftermove()

            if self.game_status != STATUS_RUNNING:
                break

            # AI moves
            if (self.turn == 1 and self.player1 == "AI") or (self.turn == 2 and self.player2 == "AI"):
                api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
                self.aftermove()

            pygame.time.delay(100)

        return self.game_status
