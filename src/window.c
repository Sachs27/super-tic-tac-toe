#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "sstate.h"


enum {
    INIT_GLEW_FAILED = 1,
    INIT_GLFW_FAILED,
    CREATE_WINDOW_FAILED,
    LOAD_TEXTURE_FAILED,
};


struct game *game;


static void on_glfw_error(int error, const char *desc)
{
    fputs(desc, stderr);
}

static void on_cursor_pos(GLFWwindow *window, double x, double y)
{
    game->cursor_xpos = x;
    game->cursor_ypos = y;
}

static void on_mouse_button(GLFWwindow *window,
                            int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        game->ismouse_clicked = 1;
        game->cursor_clicked_xpos = game->cursor_xpos;
        game->cursor_clicked_ypos = game->cursor_ypos;
    }
}

static void on_window_size(GLFWwindow *window, int width, int height)
{
    game->window_width = width;
    game->window_height = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, game->window_width, game->window_height, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
}

static int init_context(void)
{
    glfwSetErrorCallback(on_glfw_error);
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return INIT_GLFW_FAILED;
    }

    game->window = glfwCreateWindow(game->window_width,
                                    game->window_height,
                                    game->window_title, NULL, NULL);
    if (!game->window) {
        glfwTerminate();
        return CREATE_WINDOW_FAILED;
    }

    glfwSetCursorPosCallback(game->window, on_cursor_pos);
    glfwSetMouseButtonCallback(game->window, on_mouse_button);
    glfwSetWindowSizeCallback(game->window, on_window_size);
    glfwMakeContextCurrent(game->window);

    return 0;
}

static int init_gl(void)
{
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW.\n");
        return INIT_GLEW_FAILED;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, game->window_width, game->window_height, 0.0, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    return 0;
}

static int setup(void)
{
    int err;

    if ((err = init_context()) != 0)
        return err;

    if ((err = init_gl()) != 0)
        return err;

    if ((err = game_load_textures(game)) != 0)
        return err;

    return 0;
}

static void cleanup(void)
{
    glfwTerminate();
}

int main(void)
{
    game = game_create();

    if (setup() != 0) {
        return 1;
    }

    while (!glfwWindowShouldClose(game->window)) {
        game_mainloop(game, 0.0);
        glfwSwapBuffers(game->window);
        glfwPollEvents();
    }

    cleanup();

    game_destroy(game);

    return 0;
}