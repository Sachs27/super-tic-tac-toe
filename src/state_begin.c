#include "game.h"
#include "states.h"


void state_begin_update(double delta) {
    if (game->mouse.lb.isclicked) {
        game->mouse.lb.isclicked = 0;

        super_chessboard_reset(game->scb);
        sstate_push(game->ss, state_main_update, state_main_render);
    }
}

void state_begin_render() {
    glClear(GL_COLOR_BUFFER_BIT);
    texture_draw(game->tex_welcome, 0, 0);
}
