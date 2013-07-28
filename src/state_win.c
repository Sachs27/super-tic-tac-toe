#include "game.h"


void state_win_update(double delta)
{
}

void state_win_render()
{
    switch (game->winner) {
    case CHESSBOARD_PLAYER1:
        texture_draw(game->tex_win_player1, 0, 0);
        break;
    case CHESSBOARD_PLAYER2:
        texture_draw(game->tex_win_player2, 0, 0);
        break;
    }
}

