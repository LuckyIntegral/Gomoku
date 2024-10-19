#pragma once

#include <iostream>
#include <vector>

using namespace std;

#define PLAYER_1 'X'
#define PLAYER_2 'O'
#define EMPTY '-'

typedef struct s_point {
    int row, col;
}   t_point;

class Game
{
    private:
        vector<vector<u_char>>  _board;
        bool                    _is_game_over;
    public:
        Game();
        ~Game();
        const bool                      is_valid_move(t_point point, u_char player) const;
        void                            make_move(t_point point, u_char player);
        const vector<vector<u_char>>&   get_board() const;
        void                            set_game_over();
        const bool                      get_game_over() const;
};

Game::Game()
{
    this->_board = vector(19, vector<u_char>(19, '-'));
    this->_is_game_over = false;
}