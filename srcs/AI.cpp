#include "AI.hpp"

AI::AI()
{
}

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
    std::copy(this->_game->get_board().begin(), this->_game->get_board().end(), back_inserter(this->_board));
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
        score += this->count_array_on_board(array, player) * WIN_WEIGHT;
    }

    for (auto &array : FOUR_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * FOUR_UNCOVERED_WEIGHT;
    }

    for (auto &array : THREE_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * THREE_UNCOVERED_WEIGHT;
    }

    for (auto &array : TWO_UNCOVERED)
    {
        score += this->count_array_on_board(array, player) * TWO_UNCOVERED_WEIGHT;
    }

    for (auto &array : FOUR_COVERED)
    {
        score += this->count_array_on_board(array, player) * FOUR_COVERED_WEIGHT;
    }

    for (auto &array : THREE_COVERED)
    {
        score += this->count_array_on_board(array, player) * THREE_COVERED_WEIGHT;
    }
    // TODO add capture weight
    return score;
}

// Function to get the best move using minimax algorithm
const t_point AI::minimax(const u_char player, u_short depth, long alpha, long beta, bool is_maximizing)
{
    vector<t_point> possible_moves;
    t_point best_move;
    u_char opponent = player == PLAYER ? OPPONENT : PLAYER;
    long best_score;

    if (depth == 0)
    {
        best_score = this->get_score(player);
        return best_move;
    }

    set<pair<u_short, u_short>> active_cells;

    for (u_short i = 0; i < 19; i++)
    {
        for (u_short j = 0; j < 19; j++)
        {
            if (this->_board[i][j] != EMPTY)
            {
                for (short di = -1; di <= 1; di++)
                {
                    for (short dj = -1; dj <= 1; dj++)
                    {
                        if (di == 0 && dj == 0) continue;
                        short ni = (short)i + di;
                        short nj = (short)j + dj;
                        if (ni >= 0 && ni < 19 && nj >= 0 && nj < 19 && this->_board[ni][nj] == EMPTY)
                        {
                            if (this->_game->is_valid_move({ni, nj, 0}, player))
                                active_cells.insert({ni, nj});
                        }
                    }
                }
            }
        }
    }

    this->_game = this->_game->deep_copy();

    if (is_maximizing)
    {
        best_score = -WIN_WEIGHT;
        for (const auto& cell : active_cells)
        {
            u_short i = cell.first;
            u_short j = cell.second;
            this->_game->make_move({i, j, 0}, player);
            u_char opponent = player == PLAYER ? OPPONENT : PLAYER;
            long score = this->minimax(opponent, depth - 1, alpha, beta, false).score;
            if (score > best_score)
            {
                best_score = score;
                best_move.row = i;
                best_move.col = j;
            }
            alpha = max(alpha, best_score);
            if (beta <= alpha)
                break;
        }
    }
    else
    {
        best_score = WIN_WEIGHT;
        for (const auto& cell : active_cells)
        {
            u_short i = cell.first;
            u_short j = cell.second;
            // TODO replace with make_move
            this->_game->make_move({i, j, 0}, player);
            opponent = player == PLAYER ? OPPONENT : PLAYER;
            long score = this->minimax(opponent, depth - 1, alpha, beta, true).score;
            if (score < best_score)
            {
                best_score = score;
                best_move.row = i;
                best_move.col = j;
            }
            beta = min(beta, best_score);
            if (beta <= alpha)
                break;
        }
    }
    delete this->_game;
    this->_game = nullptr;
    best_move.score = best_score;
    return best_move;
}