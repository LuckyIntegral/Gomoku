#include "Game.hpp"

static void drawCircle(
    float cx,
    float cy,
    float r,
    int num_segments,
    u_char player
) {
    if (player == PLAYER_1) {
        glColor3f(BLACK);
    } else {
        glColor3f(WHITE);
    }

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= num_segments; i++) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

static void display_splited_grid(
    const int board_size,
    const float line_width = 1.0f
) {
    const float MARGIN = 0.2f;
    const float BORDER = (2.0f - 2 * MARGIN) / (board_size - 1);

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(line_width);

    glBegin(GL_LINES);
    for (int i = 0; i < board_size; i++) {
        glVertex2f(-1 + MARGIN + BORDER * i, -1 + MARGIN);
        glVertex2f(-1 + MARGIN + BORDER * i, 1 - MARGIN);
        glVertex2f(-1 + MARGIN, -1 + MARGIN + BORDER * i);
        glVertex2f(1 - MARGIN, -1 + MARGIN + BORDER * i);
    }
    glEnd();
}

auto display_moves(const vector<vector<u_char>> &board) -> void {
    const int board_size = BOARD_SIZE;
    const float MARGIN = 0.2f;
    const float BORDER = (2.0f - 2 * MARGIN) / (board_size - 1);

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (board[i][j] == EMPTY) {
                continue;
            }
            float x = -1 + MARGIN + BORDER * i;
            float y = -1 + MARGIN + BORDER * j;
            drawCircle(x, y, 0.03f, 10, board[i][j]);
        }
    }
}

void drawRectangle(float x1, float y1, float x2, float y2) {
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void    Game::display(void) const {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(213.0f / 255.0f, 169.0f / 255.0f, 94.0f / 255.0f);
    drawRectangle(-1, -1, 1, 1);

    display_splited_grid(BOARD_SIZE);
    display_splited_grid(4, 3.0f);
    display_moves(this->_board);

    glfwSwapBuffers(glfwGetCurrentContext());
}

// void renderText(const char* text, float x, float y) {
//     glRasterPos2f(x, y);
//     for (const char* c = text; *c != '\0'; c++) {
//         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
//     }
// }

// void show_player_mode_menu() {
//     glClear(GL_COLOR_BUFFER_BIT);

//     glColor3f(1.0f, 1.0f, 1.0f); // White color
//     drawRectangle(-0.6f, 0.45f, 0.6f, 0.35f); // Top frame
//     drawRectangle(-0.6f, 0.35f, 0.6f, 0.25f); // Title frame
//     drawRectangle(-0.6f, 0.25f, 0.6f, 0.15f); // Option 1 frame
//     drawRectangle(-0.6f, 0.15f, 0.6f, 0.05f); // Option 2 frame

//     // Draw text
//     glColor3f(0.0f, 0.0f, 0.0f); // Black color for text
//     renderText("Game Mode:", -0.15f, 0.38f);
//     glColor3f(1.0f, 0.0f, 0.0f); // Red color for option 1
//     renderText("1. Player vs AI", -0.15f, 0.18f);
//     glColor3f(0.0f, 0.0f, 1.0f); // Blue color for option 2
//     renderText("2. Player vs Player", -0.15f, 0.08f);

//     glfwSwapBuffers(glfwGetCurrentContext());
// }

// t_players_mode prompt_players_mode(GLFWwindow* window) {
//     while (!glfwWindowShouldClose(window)) {
//         show_player_mode_menu();
//         glfwPollEvents();
//     }
//     return PLAYER_VS_AI;
// }
