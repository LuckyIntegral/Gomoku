#pragma once

#include <iostream>
#include <vector>
#include "constants.hpp"

using namespace std;

#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define EMPTY '-'

typedef struct s_point {
    int row, col;
    long score;
}   t_point;

class Game
{
    private:
        vector<vector<u_char>>  _board;
        bool                    _is_game_over;
        u_short                 _player_1_capture;
        u_short                 _player_2_capture;
        const bool              _is_capture(const t_point point, const u_char player) const;
        u_short                 _count_three_uncovered(const u_char player) const;
        u_short                 _remove_captured_stones(const t_point point, const u_char player);
    public:
        Game();
        ~Game();
        const bool                      is_valid_move(const t_point point, const u_char player);
        u_short                         make_move(const t_point point, const u_char player);
        const vector<vector<u_char>>&   get_board() const;
        void                            set_game_over();
        const bool                      get_game_over() const;
        Game*                           deep_copy() const;
};

