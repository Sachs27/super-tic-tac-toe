#include <stdlib.h>

#include "super_chessboard.h"


struct super_chessboard *super_chessboard_create()
{
    int i, j;
    struct super_chessboard *scb;

    scb = malloc(sizeof(*scb));
    scb->thumb = chessboard_create();

    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            scb->cbs[i][j] = chessboard_create();

    super_chessboard_reset(scb);

    return scb;
}

int super_chessboard_check(struct super_chessboard *scb, int xpos, int ypos)
{
    int x = xpos / 3;
    int y = ypos / 3;

    /* input position is not the expected one. */
    if ((scb->xexpected != -1 || scb->yexpected != -1)
        && (x != scb->xexpected || y != scb->yexpected)) {
        return SUPER_CHESSBOARD_INVALID_INPUT;
    }

    return 0;
}

int super_chessboard_putchess(struct super_chessboard *scb,
                              int player, int xpos, int ypos)
{
    int err;
    int x = xpos / 3;
    int y = ypos / 3;

    int xx = xpos % 3;
    int yy = ypos % 3;

    err = super_chessboard_check(scb, xpos, ypos);
    if (err != 0)
        return err;

    err = chessboard_putchess(scb->cbs[y][x], player, xx, yy);
    if (err != 0)
        return err;

    /* change the thumbnail of the super-chessboard */
    scb->thumb->lattices[y][x] = scb->cbs[y][x]->state;

    if (chessboard_canput(scb->cbs[yy][xx])) {
        scb->xexpected = xx;
        scb->yexpected = yy;
    } else {
        scb->xexpected = -1;
        scb->yexpected = -1;
    }

    return 0;
}

void super_chessboard_reset(struct super_chessboard *scb)
{
    int i, j;

    scb->xexpected = -1;
    scb->yexpected = -1;

    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            chessboard_reset(scb->cbs[i][j]);

    chessboard_reset(scb->thumb);
}

int super_chessboard_winner(struct super_chessboard *scb)
{
    return chessboard_winner(scb->thumb);
}

