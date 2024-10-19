#pragma once

#include <vector>
#include "Game.hpp"
#include "constants.hpp"

#define PLAYER '1'
#define OPPONENT '2'
#define EMPTY '-'

// 1 - current player
// 2 - opponent

class AI
{
    private:
        Game                    *_game;
        vector<vector<u_char>>  _board;
    public:
        AI(Game *game);
        ~AI();
        const t_point                   get_best_move(const u_char player) const;
        const u_short                   count_array_on_board(const vector<u_char>& array, const u_char player) const;
        void                            create_deep_copy_board();
        const vector<vector<u_char>>    get_converted_board(const u_char player) const;
        const long                      get_score(const u_char player) const;

};

AI::AI(Game *game)
{
    this->_game = game;
    this->_board = vector(19, vector<u_char>(19, EMPTY));
}

AI::~AI()
{
}

// Function to create a deep copy of the board
void AI::create_deep_copy_board()
{
    for (u_short i = 0; i < 19; i++)
    {
        for (u_short j = 0; j < 19; j++)
        {
            this->_board[i][j] = this->_game->get_board()[i][j];
        }
    }
}

// Function to convert the board to the format that is easier to work with
const vector<vector<u_char>> AI::get_converted_board(const u_char player) const
{
    vector<vector<u_char>> converted_board = vector(19, vector<u_char>(19, EMPTY));

    for (u_short i = 0; i < 19; i++)
    {
        for (u_short j = 0; j < 19; j++)
        {
            if (this->_board[i][j] == player)
                converted_board[i][j] = PLAYER;
            else if (this->_board[i][j] == EMPTY)
                converted_board[i][j] = EMPTY;
            else
                converted_board[i][j] = OPPONENT;
        }
    }
    return converted_board;
}

// Function to count occurences of the patterns to get the overall score of the current move
const u_short AI::count_array_on_board(const vector<u_char>& array, const u_char player) const
{
    u_short count = 0;

    vector<u_char> temp_array;
    vector<vector<u_char>> temp_board = this->get_converted_board(player);

    // counting horizontally
    for (u_short i = 0; i < 19; i++)
    {
        temp_array = vector(this->_board[i].begin(), this->_board[i].begin() + array.size());
        if (temp_array == array)
            count++;
        for (u_short j = array.size(); j <= temp_board.size(); ++j)
        {
            temp_array.erase(temp_array.begin());
            if (j == array.size())
                temp_array.push_back(OPPONENT);
            else
                temp_array.push_back(temp_board[i][j]);
            if (temp_array == array)
                count++;
        }
    }
    // counting vertically
    for (u_short i = 0; i < 19; i++)
    {
        temp_array.clear();
        for (u_short j = 0; j < array.size(); ++j)
        {
            temp_array.push_back(temp_board[j][i]);
        }
        if (temp_array == array)
            count++;
        for (u_short j = array.size(); j <= temp_board.size(); ++j)
        {
            temp_array.erase(temp_array.begin());
            if (j == array.size())
                temp_array.push_back(OPPONENT);
            else
                temp_array.push_back(temp_board[j][i]);
            if (temp_array == array)
                count++;
        }
    }
    // counting left to right diagonally
    for (u_short i = 0; i < 19; i++)
    {
        temp_array.clear();
        for (u_short j = 0; j < array.size(); ++j)
        {
            if (i + j < 19)
                temp_array.push_back(temp_board[j][i + j]);
            else
                temp_array.push_back(OPPONENT);
        }
        if (temp_array == array)
            count++;
        for (u_short j = array.size(); j <= temp_board.size(); ++j)
        {
            temp_array.erase(temp_array.begin());
            if (j == temp_board.size())
            {
                temp_array.push_back(OPPONENT);
            }
            else
            {
                if (i + j < 19)
                    temp_array.push_back(temp_board[j][i + j]);
                else
                    temp_array.push_back(OPPONENT);
            }
            if (temp_array == array)
                count++;
        }
    }
    // counting right to left diagonally
    for (u_short i = 0; i < 19; i++)
    {
        temp_array.clear();
        for (u_short j = 0; j < array.size(); ++j)
        {
            if (i - j >= 0)
                temp_array.push_back(temp_board[j][i - j]);
            else
                temp_array.push_back(OPPONENT);
        }
        if (temp_array == array)
            count++;
        for (u_short j = array.size(); j <= temp_board.size(); ++j)
        {
            temp_array.erase(temp_array.begin());
            if (j == temp_board.size())
            {
                temp_array.push_back(OPPONENT);
            }
            else
            {
                if (i - j >= 0)
                    temp_array.push_back(temp_board[j][i - j]);
                else
                    temp_array.push_back(OPPONENT);
            }
            if (temp_array == array)
                count++;
        }
    }
    return count;
}

// Function to calculate score for minimax algorithm
const long AI::get_score(const u_char player) const
{
    long score = 0;

    for (auto &array : WIN)
    {
        score += this->count_array_on_board(array, player) * 1000000000;
    }

    for (auto &array : FOUR_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * 1000000;
    }

    for (auto &array : THREE_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * 1000;
    }

    for (auto &array : TWO_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * 100;
    }

    for (auto &array : FOUR_COVERED)
    {
        score += this->count_array_on_board(array, player) * 10000;
    }

    for (auto &array : THREE_COVERED)
    {
        score += this->count_array_on_board(array, player) * 1000;
    }

    return score;
}