# from Constants import EMPTY, PLAYER1, PLAYER2, WIN_WEIGHT
import game_module as gm
import time

from colorama import Fore, Style, init

PLAYER1 = 1
PLAYER2 = 2
EMPTY = 0

init(autoreset=True)
def print_board(board):
    for row in board:
        row_str = ""
        for cell in row:
            if cell == EMPTY:
                row_str += " 0 "
            elif cell == PLAYER1:
                row_str += " " + Fore.RED + "1" + Fore.RESET + " "
            else:
                row_str += " " + Fore.BLUE + "2" + Fore.RESET + " "
        print(row_str)

def main():
    game = gm.Game()
    current_player = PLAYER1

    count = 0
    while True:
        count += 1
        print(f"Player {current_player}'s turn")
        if count % 2 == 0:
            ai = gm.AI(game, current_player)  # Assuming you have AI class in Python that uses game_module
            start = time.time()
            score, move = ai.iterativeDeepening(current_player, 10)
            print(f"Score: {score}")
            print(f"Time taken: {time.time() - start}")
            if move is not None:
                captures = 0
                captures = game.makeMove(current_player, move[0], move[1], captures, [])
                print(f"Player {current_player} made a move at ({move[0]}, {move[1]}) and captured {captures} stones.")
            else:
                print("No move available.")
            current_player = PLAYER2 if current_player == PLAYER1 else PLAYER1
            print_board(game.getBoard())
            time.sleep(4)
            continue
        else:
            ai = gm.AI(game, current_player)  # Assuming you have AI class in Python that uses game_module
            start = time.time()
            score, move = ai.iterativeDeepening(current_player, 10)
            print(f"Score: {score}")
            print(f"Time taken: {time.time() - start}")
            if move is not None:
                captures = 0
                captures = game.makeMove(current_player, move[0], move[1], captures, [])
                print(f"Player {current_player} made a move at ({move[0]}, {move[1]}) and captured {captures} stones.")
            else:
                print("No move available.")
            current_player = PLAYER2 if current_player == PLAYER1 else PLAYER1
            print_board(game.getBoard())
            time.sleep(4)
            continue
        try:
            row = int(input("Enter row (0-18): "))
            col = int(input("Enter col (0-18): "))
        except ValueError:
            print("Invalid input. Please enter numbers between 0 and 18.")
            continue

        if not game.isValidMove(current_player, row, col):
            print("Invalid move. Try again.")
            count -= 1
            continue

        captures = game.makeMove(current_player, row, col, 0, [])
        print(f"Player {current_player} made a move at ({row}, {col}) and captured {captures} stones.")

        best_moves = game.getBestPossibleMoves(current_player)
        print(f"Best possible moves for Player {current_player}: {best_moves}")
        print(game.getBoard())

        # Switch player
        current_player = PLAYER2 if current_player == PLAYER1 else PLAYER1

if __name__ == "__main__":
    main()
