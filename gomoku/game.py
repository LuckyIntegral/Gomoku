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
DEPTH_HINTS = 3
DEPTH = 11

TURNS_TO_DISPLAY = [
    "Player 1's turn",
    "Player 2's turn",
]

OPPONENT = 3

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

        if setup["mode"] == game_setup.OPTION_PLAYER_VS_PLAYER_HINTS:
            setup["mode"] = game_setup.OPTION_PLAYER_VS_PLAYER

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
        ''' Switch the turn | 1 -> 2 | 2 -> 1 | '''
        self.turn = OPPONENT - self.turn


    def refresh_status(self) -> None:
        ''' Refresh the game status '''
        if self.game.get_captures(1) >= 5:
            self.game_status = STATUS_WIN_PLAYER1
        elif self.game.get_captures(2) >= 5:
            self.game_status = STATUS_WIN_PLAYER2
        elif api.is_draw(self.game.get_board()):
            self.game_status = STATUS_DRAW
        else:
            self.game_status = STATUS_RUNNING


    def sync_display(self, turn_to_display: str) -> None:
        ''' Sync the display with the board '''
        display.draw_board(self.screen)
        display.draw_pieces(self.screen, self.game.get_board(), self.hints)
        if self.hints_mode:
            for hint in self.hints:
                display.draw_piece(self.screen, hint[0], hint[1], display.HINT)
        display.draw_menu(self.screen, turn_to_display, self.setup["start_rules"], self.player1, self.capt[0], self.player2, self.capt[1], self.get_time() - self.clock)
        pygame.display.flip()


    def aftermove(self, turn_to_display: str) -> None:
        ''' After move '''
        if self.hints_mode:
            self.hints = []
        self.capt[self.turn - 1] = self.game.get_captures(self.turn)
        self.swtich_move()
        self.refresh_status()
        self.sync_display(turn_to_display)
        self.clock = self.get_time()
        if self.hints_mode:
            self.add_player_hints()
            display.draw_pieces(self.screen, self.game.get_board(), self.hints)
            pygame.display.flip()



    def first_stronger(self) -> bool:
        ''' Evaluate the board '''
        return True


    def outplay_swap_opening(self, swap2: bool) -> None:
        ''' Outplay the swap opening
        First player playes three stones and than the second player decides:
        - if he wants to keep the position
        - if he wants to swap
        - if he wants to play two more stones (if swap2 is True)
        '''
        if self.player1 == "AI":
            self.game.make_move(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.swtich_move()
            self.game.make_move(self.turn, 8, 8, self.capt[self.turn - 1], [])
            self.swtich_move()
            self.game.make_move(self.turn, 9, 10, self.capt[self.turn - 1], [])
            self.aftermove(TURNS_TO_DISPLAY[1])
            if swap2:
                option = self.request_window(["Start White", "Continue Black", "Continue swap"])
            else:
                option = self.request_window(["Start White", "Continue Black"])

            if option == 0:
                api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
                self.aftermove(TURNS_TO_DISPLAY[0])
            elif option == 2:
                self.sync_display(TURNS_TO_DISPLAY[1])
                moved = 0

                while moved < 2:
                    for event in pygame.event.get():
                        if event.type == pygame.QUIT: exit(0)
                        if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                            move, inside = display.mouse_click(self.game.get_board())
                            if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                                self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                                moved += 1
                                self.aftermove(TURNS_TO_DISPLAY[1])
                    pygame.time.delay(100)
            if option != 1:
                self.player1 = "Player"
                self.player2 = "AI"
        else:
            moved = 0

            while moved < 3:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.get_board())
                        if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                            self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            moved += 1
                            self.aftermove(TURNS_TO_DISPLAY[0])
                pygame.time.delay(100)

            if self.first_stronger():
                self.player1 = "AI"
                self.player2 = "Player"


    def outplay_pro_opening(self, dist: int) -> None:
        if self.player1 == "AI":
            self.game.make_move(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.aftermove(TURNS_TO_DISPLAY[2 - self.turn])
            move = (-1, -1)

            while move[0] == -1:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.get_board())
                        if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                            self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove(TURNS_TO_DISPLAY[2 - self.turn])
                pygame.time.delay(100)

            if move[0] <= 9 and self.game.get_board()[9-dist][9] == 0:
                self.game.make_move(self.turn, 9-dist, 9, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[0] >= 9 and self.game.get_board()[9+dist][9] == 0:
                self.game.make_move(self.turn, 9+dist, 9, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[1] <= 9 and self.game.get_board()[9][9-dist] == 0:
                self.game.make_move(self.turn, 9, 9-dist, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[1] >= 9 and self.game.get_board()[9][9+dist] == 0:
                self.game.make_move(self.turn, 9, 9+dist, self.capt[self.turn - 1], [])
                self.swtich_move()

        else:
            self.game.make_move(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.swtich_move()
            api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
            self.aftermove(TURNS_TO_DISPLAY[0])
            moved = False

            while not moved:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.get_board())
                        if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                            if 9-dist < move[0] < 9+dist and 9-dist < move[1] < 9+dist:
                                continue
                            self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove(TURNS_TO_DISPLAY[2 - self.turn])
                            moved = True
                pygame.time.delay(100)


    def outplay_opening(self) -> None:
        ''' If the start rules are not standard, play the opening '''
        if self.setup['start_rules'] == game_setup.OPTION_SWAP:
            self.outplay_swap_opening(swap2=False)
        elif self.setup['start_rules'] == game_setup.OPTION_SWAP2:
            self.outplay_swap_opening(swap2=True)
        elif self.setup['start_rules'] == game_setup.OPTION_PRO:
            self.outplay_pro_opening(3)
        elif self.setup['start_rules'] == game_setup.OPTION_LONG_PRO:
            self.outplay_pro_opening(4)


    def request_window(self, options: list[str]) -> int:
        ''' Request the window '''
        return display.request_window(self.screen, options)


    def control_opening(self) -> None:
        ''' Control the opening '''
        def hotseat_pro(dist: int):
            moved = 0
            while moved < 3:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.get_board())
                        if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                            if moved == 0:
                                if move[0] != 9 or move[1] != 9:
                                    continue
                            if moved == 2:
                                if 9-dist < move[0] < 9+dist and 9-dist < move[1] < 9+dist:
                                    continue
                            self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove(TURNS_TO_DISPLAY[self.turn - 1])
                            moved += 1
                pygame.time.delay(100)

        def swap(swap2: bool = False):
            def request_moves(n: int, player: str):
                moved = 0
                while moved < n:
                    for event in pygame.event.get():
                        if event.type == pygame.QUIT: exit(0)
                        if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                            move, inside = display.mouse_click(self.game.get_board())
                            if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                                self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                                self.aftermove(player)
                                moved += 1
                    pygame.time.delay(100)

            request_moves(3, TURNS_TO_DISPLAY[0])
            if swap2:
                option = self.request_window(["Start White", "Continue Black", "Continue swap"])
            else:
                option = self.request_window(["Start White", "Continue Black"])
            self.sync_display(TURNS_TO_DISPLAY[1])
            if option == 2:
                request_moves(2, TURNS_TO_DISPLAY[1])
                self.request_window(["Start White", "Continue Black"])
            # lol, we ignore all the options and just play the game

        if self.setup['start_rules'] == game_setup.OPTION_PRO:
            hotseat_pro(3)
        elif self.setup['start_rules'] == game_setup.OPTION_LONG_PRO:
            hotseat_pro(4)
        elif self.setup['start_rules'] == game_setup.OPTION_SWAP:
            swap()
        elif self.setup['start_rules'] == game_setup.OPTION_SWAP2:
            swap(swap2=True)


    def add_player_hints(self):
        ''' Add player hints '''
        ai = gm.AI(self.game, self.turn)
        _, move = ai.iterative_deepening(self.turn, DEPTH_HINTS)
        self.hints = [[move[1], move[0]]]


    def loop(self) -> int:
        ''' Main game loop '''
        self.sync_display(TURNS_TO_DISPLAY[self.turn - 1])
        if self.setup['start_rules'] != game_setup.OPTION_STANDARD:
            if self.setup['mode'] == game_setup.OPTION_PLAYER_VS_AI:
                self.outplay_opening()
            elif self.setup['mode'] == game_setup.OPTION_PLAYER_VS_PLAYER:
                self.control_opening()
        self.sync_display(TURNS_TO_DISPLAY[self.turn - 1])

        while self.game_status == STATUS_RUNNING:
            for event in pygame.event.get():
                if event.type == pygame.QUIT: exit(0)
                if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                    # Player moves
                    if (self.turn == 1 and self.player1 == "Player")\
                            or (self.turn == 2 and self.player2 == "Player"):
                        move, inside = display.mouse_click(self.game.get_board())
                        if inside and self.game.is_valid_move(self.turn, move[0], move[1]):
                            self.game.make_move(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove(TURNS_TO_DISPLAY[2 - self.turn])
                            if api.is_win(self.game.get_board(), self.turn):
                                self.game_status = STATUS_WIN_PLAYER1 if self.turn == 1 else STATUS_WIN_PLAYER2
            if self.game_status != STATUS_RUNNING:
                break

            # AI moves
            if (self.turn == 1 and self.player1 == "AI") or (self.turn == 2 and self.player2 == "AI"):
                api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
                self.aftermove(TURNS_TO_DISPLAY[2 - self.turn])
                if api.is_win(self.game.get_board(), self.turn):
                    self.game_status = STATUS_WIN_PLAYER1 if self.turn == 1 else STATUS_WIN_PLAYER2


            pygame.time.delay(100)

        display.display_exit_status(self.screen, self.game_status)
        