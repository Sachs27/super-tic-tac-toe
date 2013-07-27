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

    free(game);
}

int game_load_textures(struct game *game)
{
    game->tex_o = texture_load("res/o.png");
    if (game->tex_o == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    game->tex_bigo = texture_load("res/bigo.png");
    if (game->tex_bigo == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    game->tex_x = texture_load("res/x.png");
    if (game->tex_o == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    game->tex_bigx = texture_load("res/bigx.png");
    if (game->tex_bigx == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    game->tex_mask = texture_load("res/mask.png");
    if (game->tex_mask == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    game->tex_welcome = texture_load("res/welcome.png");
    if (game->tex_welcome == NULL)
        return GAME_LOAD_TEXTURE_FAILED;

    return 0;
}

void game_mainloop(struct game *game, double delta)
{
    sstate_update_and_render(game->ss, 0.0);
}
