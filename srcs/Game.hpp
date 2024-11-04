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
#include "constants.hpp"

using namespace std;

#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define EMPTY '-'

#define BOARD_SIZE 19

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

typedef struct s_point {
    int row, col;
    long score;
}   t_point;

typedef enum e_player_mode {
    PLAYER_VS_PLAYER_NO_HINTS,
    PLAYER_VS_PLAYER_HINTS,
    PLAYER_VS_AI,
}   t_players_mode;

typedef enum e_gamemode {
    STANDARD,
    PRO,
    LONG_PRO,
    SWAP,
    SWAP2,
    SWAP2_PENTE,
}   t_gamemode;

typedef struct s_game_setup {
    t_players_mode player_mode;
    t_gamemode game_mode;
    bool is_player_1_first;
}   t_game_setup;

class Game
{
    private:
        vector<vector<u_char>>  _board;
        bool                    _is_game_over;
        GLFWwindow              *_window;
        t_game_setup            _game_setup;
        u_short                 _player_1_capture;
        u_short                 _player_2_capture;
        const bool              _is_capture(const t_point point, const u_char player) const;
        u_short                 _count_three_uncovered(const u_char player) const;
        u_short                 _remove_captured_stones(const t_point point, const u_char player);

    public:
        Game();
        ~Game();
        void                            init();
        void                            loop();
        void                            prompt_game_setup();
        void                            display() const;
        void                            mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        int                             get_game_setup();
        const bool                      is_valid_move(t_point point, u_char player);
        u_short                         make_move(t_point point, u_char player);
        Game*                           deep_copy() const;

        const vector<vector<u_char>>&   get_board() const;
        void                            set_game_over();
        const bool                      get_game_over() const;
        void                            handle_resize() const;
};
