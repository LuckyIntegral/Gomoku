# Gomoku
Yeah, well, your brain has to fry sometime

## Introduction
Gomoku, also known as Five in a Row, is a classic board game where the objective is to align five of your pieces in a row, either horizontally, vertically, or diagonally. This repository contains an implementation of Gomoku with various starting rule variations to add different levels of challenge and strategy to the game.

## Starting Rule Variations

### STANDARD
In the STANDARD rule, players take turns placing their pieces on the board. The first player to align five pieces in a row wins. There are no additional rules or constraints.

### PRO
The first player's first stone must be placed in the center of the board. The second player's first stone may be placed anywhere on the board. The first player's second stone must be placed at least three intersections away from the first stone (two empty intersections in between the two stones).

### LONG_PRO
The first player's first stone must be placed in the center of the board. The second player's first stone may be placed anywhere on the board. The first player's second stone must be placed at least four intersections away from the first stone (three empty intersections in between the two stones).

### SWAP
In the SWAP rule, the first player places three pieces on the board: two of their own and one of their opponent's. The second player then decides whether to swap sides or continue playing as the second player. This rule helps mitigate the first-move advantage.

### SWAP2
The SWAP2 rule is an extension of the SWAP rule. After the first player places three pieces (two of their own and one of their opponent's), the second player has three options: swap sides, place two more pieces (one of each color), or continue playing as the second player. This rule provides more flexibility and strategic depth.

### OVERLINE_RULE
In the OVERLINE_RULE, a player is not allowed to win by making more than five in a row (overline). If a player creates six or more in a row, it doesn’t count as a win.

## Additional Rules

In the context of this project, you will play with the following additional rules:

- Two players take turns placing stones of their color on an intersection of the board, and the game ends when one player manages to align five stones. Sometimes, only an alignment of 5 can win, and sometimes 5 or more is okay. In the context of this project, we will consider 5 or more to be a win.
- There are different interpretations on what the board size for Gomoku should be. In the context of this project, Gomoku will be played on a 19x19 Goban, without limit to the number of stones.
- There are a great many additional rules to Gomoku (Google it!) aimed at making the game more fair (regular Gomoku is proven to be unfair, a perfect first player wins 100% of the time) and more interesting.
- Capture (As in the Ninuki-renju or Pente variants): You can remove a pair of your opponent’s stones from the board by flanking them with your own stones. This rule adds a win condition: If you manage to capture ten of your opponent’s stones, you win the game.
- Endgame Capture:
  - A player who manages to line up five stones wins only if the opponent cannot break this line by capturing a pair.
  - If the player has already lost four pairs and the opponent can capture one more, the opponent wins by capture.
  - If there is no possibility of this happening, there is no need to continue the game.
- No double-threes: It is forbidden to play a move that introduces two free-three alignments, which would guarantee a win by alignment.

## How to Play
1. Choose a starting rule variation.
2. Players take turns placing their pieces on the board according to the chosen rule.
3. The first player to align five pieces in a row (horizontally, vertically, or diagonally) wins the game.
4. In some variations, capturing pairs of opponent’s stones is allowed, which can also contribute to winning the game.

## Installation
To install and run the game, follow these steps:
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/gomoku.git
    ```
2. Navigate to the project directory:
    ```sh
    cd gomoku
    ```
3. Build the project:
    ```sh
    make all
    ```
4. Run the game:
    ```sh
    ./Gomoku
    ```

Enjoy playing Gomoku with these exciting rule variations!
