#include "Game.hpp"

void renderText(const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}


void Game::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    static int player = 0;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        const double MARGIN = 0.2f;
        const double BOARD_LENGTH = 640;
        double x = xpos - 80, y = ypos - 80;

        if (x < 0 || y < 0 || x > BOARD_LENGTH || y > BOARD_LENGTH) {
            return;
        }

        int row = static_cast<int>(std::round((x / BOARD_LENGTH) * (BOARD_SIZE - 1)));
        int col = 18 - static_cast<int>(std::round((y / BOARD_LENGTH) * (BOARD_SIZE - 1)));

        if (this->_board[row][col] == EMPTY) {
            if (player % 2) {
                this->_board[row][col] = PLAYER_1;
            } else {
                this->_board[row][col] = PLAYER_2;
            }
            player += 1;
        }
    }
}

void Game::loop(void) {
    glfwSetWindowUserPointer(this->_window, this);
    auto mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods) {
        Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
        game->mouseButtonCallback(window, button, action, mods);
    };
    glfwSetMouseButtonCallback(this->_window, mouseButtonCallback);

    while (!glfwWindowShouldClose(this->_window)) {
        this->display();
        glfwPollEvents();
    }
}
