#include "Game.hpp"

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    Game *game = new Game();
    game->prompt_game_setup();
    game->loop();

    glfwTerminate();
    delete game;

    return 0;
}
