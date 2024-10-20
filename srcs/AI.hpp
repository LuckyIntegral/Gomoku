#pragma once

#include <vector>
#include <set>
#include "Game.hpp"
#include "constants.hpp"

#define PLAYER '1'
#define OPPONENT '2'
#define EMPTY '-'

// 1 - current player
// 2 - opponent

class Game;

class AI
{
    private:
        Game                    *_game;
        vector<vector<u_char>>  _board;
    public:
        AI();
        AI(Game *game);
        ~AI();
        const t_point                   get_best_move(const u_char player) const;
        const u_short                   count_array_on_board(const vector<u_char>& array, const u_char player) const;
        void                            create_deep_copy_board();
        const vector<vector<u_char>>    get_converted_board(const u_char player) const;
        const long                      get_score(const u_char player) const;
        const t_point                   minimax(const u_char player, u_short depth, long alpha, long beta, bool is_maximizing);

};
