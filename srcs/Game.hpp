#pragma once

#include <vector>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <cmath>
#include <sys/types.h>
#include <iostream>

#define BLACK 0.0f, 0.0f, 0.0f
#define WHITE 1.0f, 1.0f, 1.0f

using namespace std;

#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define EMPTY '-'

#define BOARD_SIZE 19

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

typedef struct s_point {
    int row, col;
}   t_point;

typedef enum e_player_mode {
    PLAYER_VS_PLAYER_NO_HINTS,
    PLAYER_VS_PLAYER_HINTS,
    PLAYER_VS_AI,
}   t_players_mode;

typedef enum e_gamemode {
    STANDARD,
    PRO,
    SWAP,
    SWAP2,
    SWAP2_PENTE,
}   t_gamemode;

typedef struct s_game_setup {
    t_players_mode player_mode;
    t_gamemode game_mode;
}   t_game_setup;

class Game
{
    private:
        vector<vector<u_char>>  _board;
        bool                    _is_game_over;
        GLFWwindow              *_window;
    public:
        Game();
        ~Game();
        void                            init();
        void                            loop();
        void                            display() const;
        void                            mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        int                             get_game_setup();
        const bool                      is_valid_move(t_point point, u_char player) const;
        void                            make_move(t_point point, u_char player);

        const vector<vector<u_char>>&   get_board() const;
        void                            set_game_over();
        const bool                      get_game_over() const;
};
