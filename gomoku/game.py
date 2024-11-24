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
DEPTH = 3


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
        self.turn = 3 - self.turn


    def refresh_status(self) -> None:
        ''' Refresh the game status '''
        if self.game.getCaptures(1) >= 5:
            self.game_status = STATUS_WIN_PLAYER1
        elif self.game.getCaptures(2) >= 5:
            self.game_status = STATUS_WIN_PLAYER2
        elif api.is_draw(self.game.getBoard()):
            self.game_status = STATUS_DRAW
        else:
            self.game_status = STATUS_RUNNING


    def sync_display(self) -> None:
        ''' Sync the display with the board '''
        display.draw_board(self.screen)
        display.draw_pieces(self.screen, self.game.getBoard(), self.hints)
        if self.hints_mode:
            for hint in self.hints:
                display.draw_piece(self.screen, hint[0], hint[1], display.HINT)
        display.draw_menu(self.screen, self.turn, self.setup["start_rules"], self.player1, self.capt[0], self.player2, self.capt[1], self.get_time() - self.clock)
        pygame.display.flip()


    def aftermove(self):
        ''' After move '''
        if self.hints_mode:
            self.hints = []
        self.capt[self.turn - 1] = self.game.getCaptures(self.turn)
        self.swtich_move()
        self.refresh_status()
        self.sync_display()
        self.clock = self.get_time()
        if self.hints_mode:
            self.add_player_hints()
            display.draw_pieces(self.screen, self.game.getBoard(), self.hints)
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
            self.game.makeMove(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.swtich_move()
            self.game.makeMove(self.turn, 8, 8, self.capt[self.turn - 1], [])
            self.swtich_move()
            self.game.makeMove(self.turn, 9, 10, self.capt[self.turn - 1], [])
            self.aftermove()
            if swap2:
                option = self.request_window(["Start White", "Continue Black", "Continue swap"])
            else:
                option = self.request_window(["Start White", "Continue Black"])

            if option == 0:
                api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
                self.aftermove()
            elif option == 2:
                self.sync_display()
                moved = 0

                while moved < 2:
                    for event in pygame.event.get():
                        if event.type == pygame.QUIT: exit(0)
                        if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                            move, inside = display.mouse_click(self.game.getBoard())
                            if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                                self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                                moved += 1
                                self.aftermove()
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
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            moved += 1
                            self.aftermove()
                pygame.time.delay(100)

            if self.first_stronger():
                self.player1 = "AI"
                self.player2 = "Player"


    def outplay_pro_opening(self, dist: int) -> None:
        if self.player1 == "AI":
            self.game.makeMove(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.aftermove()
            move = (-1, -1)

            while move[0] == -1:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove()
                pygame.time.delay(100)

            if move[0] <= 9 and self.game.getBoard()[9-dist][9] == 0:
                self.game.makeMove(self.turn, 9-dist, 9, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[0] >= 9 and self.game.getBoard()[9+dist][9] == 0:
                self.game.makeMove(self.turn, 9+dist, 9, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[1] <= 9 and self.game.getBoard()[9][9-dist] == 0:
                self.game.makeMove(self.turn, 9, 9-dist, self.capt[self.turn - 1], [])
                self.swtich_move()
            elif move[1] >= 9 and self.game.getBoard()[9][9+dist] == 0:
                self.game.makeMove(self.turn, 9, 9+dist, self.capt[self.turn - 1], [])
                self.swtich_move()

        else:
            self.game.makeMove(self.turn, 9, 9, self.capt[self.turn - 1], [])
            self.swtich_move()
            api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
            self.aftermove()
            moved = False

            while not moved:
                for event in pygame.event.get():
                    if event.type == pygame.QUIT: exit(0)
                    if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            if 9-dist < move[0] < 9+dist and 9-dist < move[1] < 9+dist:
                                continue
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove()
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
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            if moved == 0:
                                if move[0] != 9 or move[1] != 9:
                                    continue
                            if moved == 2:
                                if 9-dist < move[0] < 9+dist and 9-dist < move[1] < 9+dist:
                                    continue
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove()
                            moved += 1
                pygame.time.delay(100)
        if self.setup['start_rules'] == game_setup.OPTION_PRO:
            hotseat_pro(3)
        elif self.setup['start_rules'] == game_setup.OPTION_LONG_PRO:
            hotseat_pro(4)


    def add_player_hints(self):
        ''' Add player hints '''
        ai = gm.AI(self.game, self.turn)
        _, move = ai.iterativeDeepening(self.turn, DEPTH_HINTS)
        self.hints = [[move[1], move[0]]]


    def loop(self) -> int:
        ''' Main game loop '''
        self.sync_display()
        if self.setup['start_rules'] != game_setup.OPTION_STANDARD:
            if self.setup['mode'] == game_setup.OPTION_PLAYER_VS_AI:
                self.outplay_opening()
            elif self.setup['mode'] == game_setup.OPTION_PLAYER_VS_PLAYER:
                self.control_opening()
        self.sync_display()

        while self.game_status == STATUS_RUNNING:
            for event in pygame.event.get():
                if event.type == pygame.QUIT: exit(0)
                if event.type == pygame.MOUSEBUTTONDOWN and event.button == pygame.BUTTON_LEFT:
                    # Player moves
                    if (self.turn == 1 and self.player1 == "Player")\
                            or (self.turn == 2 and self.player2 == "Player"):
                        move, inside = display.mouse_click(self.game.getBoard())
                        if inside and self.game.isValidMove(self.turn, move[0], move[1]):
                            self.game.makeMove(self.turn, move[0], move[1], self.capt[self.turn - 1], [])
                            self.aftermove()
                            if api.is_win(self.game.getBoard(), self.turn):
                                self.game_status = STATUS_WIN_PLAYER1 if self.turn == 1 else STATUS_WIN_PLAYER2
            if self.game_status != STATUS_RUNNING:
                break

            # AI moves
            if (self.turn == 1 and self.player1 == "AI") or (self.turn == 2 and self.player2 == "AI"):
                api.ai_move(self.game, self.turn, DEPTH, self.capt[self.turn - 1])
                self.aftermove()
                if api.is_win(self.game.getBoard(), self.turn):
                    self.game_status = STATUS_WIN_PLAYER1 if self.turn == 1 else STATUS_WIN_PLAYER2


            pygame.time.delay(100)

        display.display_exit_status(self.screen, self.game_status)
