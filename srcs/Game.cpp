#include "Game.hpp"

Game::Game()
{
    this->_board = vector(19, vector<u_char>(19, '-'));
    this->_is_game_over = false;
}

Game::~Game()
{
}

// Function to create a deep copy of the game
Game* Game::deep_copy() const
{
    Game *new_game = new Game();
    std::copy(this->_board.begin(), this->_board.end(), back_inserter(new_game->_board));
    new_game->_is_game_over = this->_is_game_over;
    return new_game;
}

const vector<vector<u_char>>& Game::get_board() const
{
    return this->_board;
}

void Game::set_game_over()
{
    this->_is_game_over = true;
}

const bool Game::get_game_over() const
{
    return this->_is_game_over;
}

// Function to check if the move is valid
const bool Game::is_valid_move(const t_point point, const u_char player)
{
    u_char opponent = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
    bool is_capture = false;

    if (point.row < 0 || point.row >= 19 || point.col < 0 || point.col >= 19)
        return false;
    if (this->_board[point.row][point.col] != EMPTY)
        return false;
    // check if there is a capture of the pair of the opponent's stones
    if (this->_is_capture(point, player))
        return true;
    u_short three_uncovered = this->_count_three_uncovered(player);
    this->_board[point.row][point.col] = player;
    u_short three_uncovered_after = this->_count_three_uncovered(player);
    this->_board[point.row][point.col] = EMPTY;
    if (three_uncovered + 1 < three_uncovered_after)
        return false;
    return true;
}

static const bool is_left_horizontal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.col - 3 >= 0 && board[point.row][point.col - 1] == opponent && board[point.row][point.col - 2] == opponent && board[point.row][point.col - 3] == player)
        return true;
    return false;
}

static const bool is_right_horizontal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.col + 3 < 19 && board[point.row][point.col + 1] == opponent && board[point.row][point.col + 2] == opponent && board[point.row][point.col + 3] == player)
        return true;
    return false;
}

static const bool is_up_vertical_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row - 3 >= 0 && board[point.row - 1][point.col] == opponent && board[point.row - 2][point.col] == opponent && board[point.row - 3][point.col] == player)
        return true;
    return false;
}

static const bool is_down_vertical_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row + 3 < 19 && board[point.row + 1][point.col] == opponent && board[point.row + 2][point.col] == opponent && board[point.row + 3][point.col] == player)
        return true;
    return false;
}

static const bool is_left_up_diagonal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row - 3 >= 0 && point.col - 3 >= 0 && board[point.row - 1][point.col - 1] == opponent && board[point.row - 2][point.col - 2] == opponent && board[point.row - 3][point.col - 3] == player)
        return true;
    return false;
}

static const bool is_right_up_diagonal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row - 3 >= 0 && point.col + 3 < 19 && board[point.row - 1][point.col + 1] == opponent && board[point.row - 2][point.col + 2] == opponent && board[point.row - 3][point.col + 3] == player)
        return true;
    return false;
}

static const bool is_left_down_diagonal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row + 3 < 19 && point.col - 3 >= 0 && board[point.row + 1][point.col - 1] == opponent && board[point.row + 2][point.col - 2] == opponent && board[point.row + 3][point.col - 3] == player)
        return true;
    return false;
}

static const bool is_right_down_diagonal_capture(const vector<vector<u_char>>& board, const t_point point, const u_char player, const u_char opponent)
{
    if (point.row + 3 < 19 && point.col + 3 < 19 && board[point.row + 1][point.col + 1] == opponent && board[point.row + 2][point.col + 2] == opponent && board[point.row + 3][point.col + 3] == player)
        return true;
    return false;
}

// Function to check if there is a capture of the pair of the opponent's stones if the stone was placed at the given point
const bool Game::_is_capture(const t_point point, const u_char player) const
{
    u_char opponent = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;

    // check if there is a capture of the pair of the opponent's stones
    // CHECK HORIZONTAL
    if (is_left_horizontal_capture(this->_board, point, player, opponent))
        return true;
    else if (is_right_horizontal_capture(this->_board, point, player, opponent))
        return true;
    // CHECK VERTICAL
    else if (is_up_vertical_capture(this->_board, point, player, opponent))
        return true;
    else if (is_down_vertical_capture(this->_board, point, player, opponent))
        return true;
    // CHECK DIAGONAL
    else if (is_left_up_diagonal_capture(this->_board, point, player, opponent))
        return true;
    else if (is_right_up_diagonal_capture(this->_board, point, player, opponent))
        return true;
    else if (is_left_down_diagonal_capture(this->_board, point, player, opponent))
        return true;
    else if (is_right_down_diagonal_capture(this->_board, point, player, opponent))
        return true;
    return false;
}

// Function count the number of three uncovered stones in a row and return the count
u_short Game::_count_three_uncovered(const u_char player) const
{
    u_short count = 0;
    vector<vector<u_char>> three_uncovered;
    vector<u_char> temp_array;
    u_char opponent = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;

    std::copy(THREE_UNCOVERED.begin(), THREE_UNCOVERED.end(), back_inserter(three_uncovered));
    for (u_short i = 0; i < three_uncovered.size(); i++)
    {
        for (u_short j = 0; j < three_uncovered[i].size(); j++)
        {
            three_uncovered[i][j] = player;
        }
    }
    // counting horizontally
    for (auto &array : three_uncovered)
    {
        for (u_short i = 0; i < 19; i++)
        {
            temp_array = vector(this->_board[i].begin(), this->_board[i].begin() + array.size());
            if (temp_array == array)
                count++;
            for (u_short j = array.size(); j <= this->_board.size(); ++j)
            {
                temp_array.erase(temp_array.begin());
                if (j == array.size())
                    temp_array.push_back(opponent);
                else
                    temp_array.push_back(this->_board[i][j]);
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
                temp_array.push_back(this->_board[j][i]);
            }
            if (temp_array == array)
                count++;
            for (u_short j = array.size(); j <= this->_board.size(); ++j)
            {
                temp_array.erase(temp_array.begin());
                if (j == array.size())
                    temp_array.push_back(opponent);
                else
                    temp_array.push_back(this->_board[j][i]);
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
                    temp_array.push_back(this->_board[j][i + j]);
                else
                    temp_array.push_back(opponent);
            }
            if (temp_array == array)
                count++;
            for (u_short j = array.size(); j <= this->_board.size(); ++j)
            {
                temp_array.erase(temp_array.begin());
                if (j == this->_board.size())
                {
                    temp_array.push_back(opponent);
                }
                else
                {
                    if (i + j < 19)
                        temp_array.push_back(this->_board[j][i + j]);
                    else
                        temp_array.push_back(opponent);
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
                    temp_array.push_back(this->_board[j][i - j]);
                else
                    temp_array.push_back(opponent);
            }
            if (temp_array == array)
                count++;
            for (u_short j = array.size(); j <= this->_board.size(); ++j)
            {
                temp_array.erase(temp_array.begin());
                if (j == this->_board.size())
                {
                    temp_array.push_back(opponent);
                }
                else
                {
                    if (i - j >= 0)
                        temp_array.push_back(this->_board[j][i - j]);
                    else
                        temp_array.push_back(opponent);
                }
                if (temp_array == array)
                    count++;
            }
        }
    }
    return count;
}

// Function that makes a move and returns the number of captures made
// TODO adapt for different game modes
u_short Game::make_move(const t_point point, const u_char player)
{
    u_short result = this->_remove_captured_stones(point, player);
    if (player == PLAYER_1)
        this->_player_1_capture += result;
    else
        this->_player_2_capture += result;

    this->_board[point.row][point.col] = player;
    return result;
}

// Function to remove the captured stones
u_short Game::_remove_captured_stones(const t_point point, const u_char player)
{
    u_char opponent = player == PLAYER_1 ? PLAYER_2 : PLAYER_1;
    u_short count = 0;
    // check if there is a capture of the pair of the opponent's stones
    // CHECK HORIZONTAL
    if (is_left_horizontal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row][point.col - 1] = EMPTY;
        this->_board[point.row][point.col - 2] = EMPTY;
        count += 1;
    }
    if (is_right_horizontal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row][point.col + 1] = EMPTY;
        this->_board[point.row][point.col + 2] = EMPTY;
        count += 1;
    }
    // CHECK VERTICAL
    if (is_up_vertical_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row - 1][point.col] = EMPTY;
        this->_board[point.row - 2][point.col] = EMPTY;
        count += 1;
    }
    if (is_down_vertical_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row + 1][point.col] = EMPTY;
        this->_board[point.row + 2][point.col] = EMPTY;
        count += 1;
    }
    // CHECK DIAGONAL
    if (is_left_up_diagonal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row - 1][point.col - 1] = EMPTY;
        this->_board[point.row - 2][point.col - 2] = EMPTY;
        count += 1;
    }
    if (is_right_up_diagonal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row - 1][point.col + 1] = EMPTY;
        this->_board[point.row - 2][point.col + 2] = EMPTY;
        count += 1;
    }
    if (is_left_down_diagonal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row + 1][point.col - 1] = EMPTY;
        this->_board[point.row + 2][point.col - 2] = EMPTY;
        count += 1;
    }
    if (is_right_down_diagonal_capture(this->_board, point, player, opponent))
    {
        this->_board[point.row + 1][point.col + 1] = EMPTY;
        this->_board[point.row + 2][point.col + 2] = EMPTY;
        count += 1;
    }

    return count;
}
