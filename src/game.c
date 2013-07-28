#include <stdlib.h>

#include "game.h"
#include "sstate.h"
#include "states.h"
#include "texture.h"
#include "super_chessboard.h"


struct game *game_create()
{
    struct game *game;

    game = malloc(sizeof(*game));

    game->window_width = 620;
    game->window_height = 620;
    game->window_title = "Super Tic Tac toe";

    game->scb= super_chessboard_create();
    game->chessboard_xoff = 40;
    game->chessboard_yoff = 40;
    game->turn = CHESSBOARD_PLAYER1;
    game->winner = CHESSBOARD_NONE;

    game->ss = sstate_create();
    sstate_push(game->ss, state_begin_update, state_begin_render);

    return game;
}

void game_destroy(struct game *game)
{
    texture_destroy(game->tex_o);
    texture_destroy(game->tex_x);
    texture_destroy(game->tex_bigo);
    texture_destroy(game->tex_bigx);
    texture_destroy(game->tex_mask);
    texture_destroy(game->tex_welcome);
    texture_destroy(game->tex_tie);
    texture_destroy(game->tex_win_player1);
    texture_destroy(game->tex_win_player2);

    free(game);
}

int game_load_textures(struct game *game)
{
#define TEXTURE_LOAD(tex) \
    do { \
        game->tex_##tex = texture_load("res/"#tex".png"); \
        if (game->tex_##tex == NULL) \
            return GAME_LOAD_TEXTURE_FAILED;\
    } while(0)

    TEXTURE_LOAD(o);
    TEXTURE_LOAD(bigo);
    TEXTURE_LOAD(x);
    TEXTURE_LOAD(bigx);
    TEXTURE_LOAD(mask);
    TEXTURE_LOAD(welcome);
    TEXTURE_LOAD(tie);
    TEXTURE_LOAD(win_player1);
    TEXTURE_LOAD(win_player2);

    return 0;
}

void game_mainloop(struct game *game, double delta)
{
    sstate_update_and_render(game->ss, 0.0);
}
