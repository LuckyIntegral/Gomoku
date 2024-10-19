#pragma once

#include <vector>
#include "Game.hpp"
#include "constants.hpp"


// 1 - current player
// 2 - opponent

class AI
{
    private:
        Game    *_game;
    public:
        AI(Game *game);
        ~AI();
        const t_point   get_best_move(u_char player);

};

AI::AI(Game *game)
{
    this->_game = game;
}

AI::~AI()
{
}

