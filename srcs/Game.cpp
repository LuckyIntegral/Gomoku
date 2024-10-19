
#include "Game.hpp"

Game::~Game() {}

Game::Game()
{
    this->_board = vector<vector<u_char>>(BOARD_SIZE, vector<u_char>(BOARD_SIZE, '-'));
    this->_is_game_over = false;
    this->_window = glfwCreateWindow(WINDOW_HEIGHT, WINDOW_HEIGHT, "Gomoku", nullptr, nullptr);

    if (!this->_window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(this->_window);
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
