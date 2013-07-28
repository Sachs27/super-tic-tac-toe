#include "game.h"


void state_end_update(double delta)
{
    if (game->ismouse_clicked) {
        game->ismouse_clicked = 0;

        sstate_pop(game->ss);
    }
}

void state_end_render()
{
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

