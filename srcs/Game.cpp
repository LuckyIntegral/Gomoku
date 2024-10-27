
#include "Game.hpp"
#include <cstdlib>

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
    _game_setup = {PLAYER_VS_PLAYER_NO_HINTS, STANDARD, true};
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

void Game::handle_resize() const
{
    int width, height;
    glfwGetFramebufferSize(this->_window, &width, &height);
    if (width != WINDOW_HEIGHT || height != WINDOW_HEIGHT) {
        cerr << "Window resizing is not supported" << endl;
        std::exit(1);
    }
}
