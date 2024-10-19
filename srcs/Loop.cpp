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

        std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float x = (xpos / width) * 2 - 1;
        float y = 1 - (ypos / height) * 2;

        std::cout << "x: " << x << " y: " << y << std::endl;

        const float MARGIN = 0.19f;
        int row = (x + 1 - MARGIN) / (2 - 2 * MARGIN) * BOARD_SIZE;
        int col = (y + 1 - MARGIN) / (2 - 2 * MARGIN) * BOARD_SIZE;

        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
            return;
        }

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
