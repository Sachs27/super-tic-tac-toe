#ifndef SUPER_CHESSBOARD_H
#define SUPER_CHESSBOARD_H


#include "chessboard.h"


enum {
    SUPER_CHESSBOARD_INVALID_INPUT = 1,
};

struct super_chessboard {
    struct chessboard *cbs[3][3];
    struct chessboard *thumb;       /* Thumbnail of current state */
    int xexpected;
    int yexpected;
};


struct super_chessboard *super_chessboard_create();

/**
 * Check a given position whether the expected one.
 *
 * The position is in the *super-chessboard's coordinate*.
 */
int super_chessboard_check(struct super_chessboard *scb, int xpos, int ypos);

int super_chessboard_putchess(struct super_chessboard *scb,
                              int player, int xpos, int ypos);

void super_chessboard_reset(struct super_chessboard *scb);

int super_chessboard_winner(struct super_chessboard *scb);

#endif
