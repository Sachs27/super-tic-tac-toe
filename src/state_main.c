#include <math.h>

#include <GL/glew.h>

#include "game.h"
#include "states.h"


enum {
    PLAYER_NOT_INPUT = 1,
    PLAYER_INPUT_INVALID,
};


extern struct game *game;


static int cursorpos_to_chessboardpos(int cursor_xpos, int cursor_ypos,
                                      int *xpos, int *ypos) {
    cursor_xpos -= game->chessboard_xoff;
    cursor_ypos -= game->chessboard_yoff;

    if (cursor_xpos < 0 || cursor_ypos < 0)
        return PLAYER_INPUT_INVALID;

    *xpos = cursor_xpos / game->tex_o->width;
    *ypos = cursor_ypos / game->tex_o->height;

    if (*xpos >= 0 && *xpos < 9 && *ypos >= 0 && *ypos < 9)
        return 0;
    else
        return PLAYER_INPUT_INVALID;
}

static int get_player_input(int player, int *xpos, int *ypos) {
    if (game->mouse.lb.isclicked) {
        game->mouse.lb.isclicked = 0;
        return cursorpos_to_chessboardpos(game->mouse.lb.x, game->mouse.lb.y,
                                          xpos, ypos);
    }

    return PLAYER_NOT_INPUT;
}

static int putchess(void) {
    int err;
    int xpos, ypos;

    if ((err = get_player_input(game->turn, &xpos, &ypos)) != 0)
        return err;

    if ((err = super_chessboard_putchess(game->scb,
                                         game->turn, xpos, ypos)) != 0)
        return err;

    /* swap player's turn */
    game->turn = (game->turn == CHESSBOARD_PLAYER1) ? CHESSBOARD_PLAYER2
                                                    : CHESSBOARD_PLAYER1;

    return 0;
}

static void drawline(GLfloat size,
                     GLfloat xp1, GLfloat yp1, GLfloat xp2, GLfloat yp2) {
    GLfloat r = size / 2.0f;
    GLfloat xline = xp2 - xp1;
    GLfloat yline = yp2 - yp1;
    GLfloat length, rotation;

    length = sqrtf(xline * xline + yline * yline);
    rotation = atan2f(yline, xline);

    glLoadIdentity();
    glTranslatef(xp1, yp1, 0.0f);
    glRotatef(rotation / M_PI * 180.f, 0.0f, 0.0f, 1.0f);

    glBegin(GL_QUADS);
    glVertex2f(0, - r);
    glVertex2f(length, - r);
    glVertex2f(length, r);
    glVertex2f(0, r);
    glEnd();
}

static void drawhint() {
    int xpos, ypos;
    int xscreen, yscreen;
    struct texture *tex;

    if (cursorpos_to_chessboardpos(game->mouse.x, game->mouse.y,
                                   &xpos, &ypos) != 0) {
        return;
    }

    if (game->turn == CHESSBOARD_PLAYER1) {
        tex = game->tex_o;
    } else {
        tex = game->tex_x;
    }

    xscreen = game->chessboard_xoff + xpos * tex->width;
    yscreen = game->chessboard_yoff + ypos * tex->height;

    if (super_chessboard_check(game->scb, xpos, ypos) == 0
            && chessboard_canput(game->scb->cbs[ypos/3][xpos/3])
            && chessboard_canput_at(game->scb->cbs[ypos/3][xpos/3],
                xpos%3, ypos%3)) {
        glColor3f(0.0f, 1.0f, 0.0f);
    } else {
        glColor3f(1.0f, 0.0f, 0.0f);
    }

    drawline(tex->width, xscreen, yscreen + tex->width / 2.0f,
             xscreen + tex->width, yscreen + tex->width / 2.0f);
    texture_draw(tex, xscreen, yscreen);
}

static void chessboard_draw(struct chessboard *cb, GLfloat xoff, GLfloat yoff) {
    int tex_width = game->tex_o->width;
    int tex_height = game->tex_o->height;
    int i, j;

    /* draw lines */
    glColor3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= 3; ++i) {
        int h = i * tex_height + yoff;
        drawline(1.0f, xoff, h, xoff + 3 * tex_width, h);
    }
    for (j = 0; j <= 3; ++j) {
        int w = j * tex_width + xoff;
        drawline(1.0f, w, yoff, w, yoff + 3 * tex_height);
    }
    /* draw chesses */
    for (i = 0; i < 3; ++i)
    for (j = 0; j < 3; ++j) {
        switch (cb->lattices[i][j]) {
        case CHESSBOARD_PLAYER1:
            texture_draw(game->tex_o, xoff + j * tex_width,
                    yoff + i * tex_height);
            break;
        case CHESSBOARD_PLAYER2:
            texture_draw(game->tex_x, xoff + j * tex_width,
                    yoff + i * tex_height);
            break;
        }
    }
}

static void chessboard_drawstate(struct chessboard *cb, int xoff, int yoff) {
    switch (cb->state) {
    case CHESSBOARD_PLAYER1:
        texture_draw(game->tex_bigo, xoff, yoff);
        break;
    case CHESSBOARD_PLAYER2:
        texture_draw(game->tex_bigx, xoff, yoff);
        break;
    }
}

static void super_chessboard_draw(struct super_chessboard *scb,
                                  GLfloat xoff, GLfloat yoff) {
    int i, j;
    int cb_width, cb_height;

    cb_width = game->tex_o->width * 3;
    cb_height = game->tex_o->height * 3;

    /* draw lines */
    glColor3f(0.0f, 0.0f, 0.0f);
    for (i = 0; i <= 3; ++i) {
        int h = i * cb_height + yoff;
        drawline(2.0f, xoff, h, xoff + 3 * cb_width, h);
    }
    for (j = 0; j <= 3; ++j) {
        int w = j * cb_width + xoff;
        drawline(2.0f, w, yoff, w, yoff + 3 * cb_height);
    }
    /* draw chesses */
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            chessboard_draw(scb->cbs[i][j], xoff + j * cb_width,
                                            yoff + i * cb_height);

    if (!game_iswin(game)) {
        drawhint();
    }

    /* draw chessboard's state */
    for (i = 0; i < 3; ++i)
    for (j = 0; j < 3; ++j) {
        chessboard_drawstate(scb->cbs[i][j], xoff + j * cb_width,
                                             yoff + i * cb_height);
    }

    /* draw mask */
    if (game->scb->xexpected == -1 || game->scb->yexpected == -1
            || game_iswin(game)) {
        return;
    }

    for (i = 0; i < 3; ++i)
    for (j = 0; j < 3; ++j) {
        if (j == game->scb->xexpected && i == game->scb->yexpected)
            continue;

        texture_draw_with_transparency(game->tex_mask,
            xoff + j * game->tex_mask->width,
            yoff + i * game->tex_mask->height, 0.5);
    }
}

void state_main_update(double delta) {
    if (game_iswin(game)) {
        if (game->mouse.lb.isclicked) {
            game->mouse.lb.isclicked = 0;
            sstate_pop(game->ss);
        }
    } else {
        putchess();
    }
}

void state_main_render() {
    glClear(GL_COLOR_BUFFER_BIT);

    super_chessboard_draw(game->scb,
                          game->chessboard_xoff,
                          game->chessboard_yoff);

    if (game_iswin(game)) {
        switch (game->winner) {
        case CHESSBOARD_PLAYER1:
            texture_draw(game->tex_win_player1, 0, 0);
            break;
        case CHESSBOARD_PLAYER2:
            texture_draw(game->tex_win_player2, 0, 0);
            break;
        case CHESSBOARD_TIE:
            texture_draw(game->tex_tie, 0, 0);
            break;
        }
    }
}
