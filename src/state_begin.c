#include "game.h"
#include "sstate.h"
#include "states.h"
#include "texture.h"
#include "super_chessboard.h"


extern struct game *game;


void state_begin_update(double delta)
{
    if (game->ismouse_clicked) {
        game->ismouse_clicked = 0;

        super_chessboard_reset(game->scb);
        sstate_push(game->ss, state_main_update, state_main_render);
    }
}

void state_begin_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    texture_draw(game->tex_welcome, 0, 0);
}
