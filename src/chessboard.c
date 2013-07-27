#include <stdlib.h>

#include "chessboard.h"


static int isown_three(struct chessboard *cb, int player,
                       int xpos, int ypos, int xstep, int ystep)
{
    int i;

    for (i = 0; i < 3; ++i, xpos += xstep, ypos += ystep) {
        if (cb->lattices[ypos][xpos] != player) {
            return 0;
        }
    }

    return 1;
}

static int isplayer_win(struct chessboard *cb, int player)
{
    /* check horizontally */
    if (isown_three(cb, player, 0, 0, 1, 0))
        return 1;
    if (isown_three(cb, player, 0, 1, 1, 0))
        return 1;
    if (isown_three(cb, player, 0, 2, 1, 0))
        return 1;
    /* check vertically */
    if (isown_three(cb, player, 0, 0, 0, 1))
        return 1;
    if (isown_three(cb, player, 1, 0, 0, 1))
        return 1;
    if (isown_three(cb, player, 2, 0, 0, 1))
        return 1;
    /* check diagonally */
    if (isown_three(cb, player, 0, 0, 1, 1))
        return 1;
    if (isown_three(cb, player, 2, 0,-1, 1))
        return 1;

    return 0;
}

static int isfull(struct chessboard *cb)
{
    int i ,j;
    int had;

    had = 0;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            if (cb->lattices[i][j] != CHESSBOARD_LATTICE_EMPTY)
                ++had;

    if (had == 0)
        return CHESSBOARD_LATTICE_EMPTY;
    else if (had < 9)
        return CHESSBOARD_LATTICE_HAVE;
    else
        return CHESSBOARD_LATTICE_FULL;
}

struct chessboard *chessboard_create()
{
    struct chessboard *cb;

    cb = malloc(sizeof(*cb));

    chessboard_reset(cb);

    return cb;
}

int chessboard_winner(struct chessboard *cb)
{
    if (isplayer_win(cb, CHESSBOARD_PLAYER1))
        return CHESSBOARD_PLAYER1;

    if (isplayer_win(cb, CHESSBOARD_PLAYER2))
        return CHESSBOARD_PLAYER2;

    return isfull(cb);
}

int chessboard_putchess(struct chessboard *cb, int player, int xpos, int ypos)
{
    if (cb->state == CHESSBOARD_LATTICE_FULL)
        return CHESSBOARD_LATTICE_FULL;

    if (cb->lattices[ypos][xpos] != CHESSBOARD_LATTICE_EMPTY)
        return CHESSBOARD_LATTICE_FULL;

    cb->lattices[ypos][xpos] = player;

    cb->state = chessboard_winner(cb);

    return 0;
}

void chessboard_reset(struct chessboard *cb)
{
    int i, j;

    if (cb->state == CHESSBOARD_LATTICE_EMPTY)
        return;

    cb->state = CHESSBOARD_LATTICE_EMPTY;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            cb->lattices[i][j] = CHESSBOARD_LATTICE_EMPTY;
}

int chessboard_canput(struct chessboard *cb)
{
    if (cb->state == CHESSBOARD_LATTICE_EMPTY
            || cb->state == CHESSBOARD_LATTICE_HAVE)
        return 1;
    else
        return 0;
}

int chessboard_canput_at(struct chessboard *cb, int xpos, int ypos)
{
    if (cb->lattices[ypos][xpos] == CHESSBOARD_LATTICE_EMPTY)
        return 1;
    else
        return 0;
}
