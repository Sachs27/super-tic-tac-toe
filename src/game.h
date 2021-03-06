#ifndef GAME_H
#define GAME_H


#include "texture.h"
#include "super_chessboard.h"
#include "sstate.h"


enum {
    GAME_LOAD_TEXTURE_FAILED = 1,
};

struct game {
    struct GLFWwindow  *window;
    int                 window_width;
    int                 window_height;
    char               *window_title;

    struct texture *tex_o;
    struct texture *tex_bigo;
    struct texture *tex_x;
    struct texture *tex_bigx;
    struct texture *tex_mask;
    struct texture *tex_welcome;
    struct texture *tex_tie;
    struct texture *tex_win_player1;
    struct texture *tex_win_player2;

    struct super_chessboard *scb;
    int chessboard_xoff;
    int chessboard_yoff;
    int turn;
    int winner;

    struct {
        int x;
        int y;
        struct {
            int isclicked;
            int x;  /* left-button clicked at */
            int y;
        } lb;
    } mouse;

    struct sstate *ss;
};


struct game *game_create();

void game_destroy(struct game *game);

int game_load_textures(struct game *game);

void game_mainloop(struct game *game, double delta);

int game_iswin(struct game *game);


extern struct game *game;


#endif /* GAME_H */
