#ifndef CHESSBOARD_H
#define CHESSBOARD_H


enum {
    CHESSBOARD_NONE = 0,
    CHESSBOARD_PLAYER1 = 1,
    CHESSBOARD_PLAYER2 = 2,
    CHESSBOARD_TIE = 3,
    CHESSBOARD_HAVE,
    CHESSBOARD_FULL,
};


struct chessboard {
    /*
     * Represent the lattice of a chessboard.
     *
     * Can be one of the following value:
     *     CHESSBOARD_NONE
     *     CHESSBOARD_PLAYER1
     *     CHESSBOARD_PLAYER2
     */
    int lattices[3][3];

    /*
     * Represent the winner of a chessboard.
     *
     * Can be one of the following value:
     *     CHESSBOARD_NONE
     *     CHESSBOARD_PLAYER1
     *     CHESSBOARD_PLAYER2
     *     CHESSBOARD_TIE
     */
    int state;
};


struct chessboard *chessboard_create();

int chessboard_winner(struct chessboard *cb);

int chessboard_putchess(struct chessboard *cb, int player, int xpos, int ypos);

void chessboard_reset(struct chessboard *cb);

int chessboard_canput(struct chessboard *cb);

int chessboard_canput_at(struct chessboard *cb, int xpos, int ypos);


#endif /* CHESSBOARD_H */
