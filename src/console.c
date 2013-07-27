#include <stdio.h>

#include "chessboard.h"


static struct chessboard *chessboard;


static void chessboard_draw(struct chessboard *cb)
{
	int i, j;

	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j)
			switch (cb->lattices[i][j]) {
			case CHESSBOARD_LATTICE_EMPTY:
				printf("*");
				break;
			case CHESSBOARD_LATTICE_PLAYER1:
				printf("o");
				break;
			case CHESSBOARD_LATTICE_PLAYER2:
				printf("x");
				break;
			}
		printf("\n");
	}
}

static void get_player_input(int player, int *xpos, int *ypos)
{
	printf("Please input player%d's decision: ", player);
	while (scanf("%d %d", xpos, ypos) != 2)
		;
}

static void make_decision(int player)
{
	int err, xpos, ypos;

retry:
	get_player_input(player, &xpos, &ypos);
	err = chessboard_putchess(chessboard,
			player, xpos, ypos);
	if (err == CHESSBOARD_HADCHESS) {
		printf("(%d, %d) is _not_ empty!\n", xpos, ypos);
		goto retry;
	}
}

int main(void)
{
	int winner;
	int turn;

	chessboard = chessboard_create();

	turn = 0;
	do {
		chessboard_draw(chessboard);

		make_decision(turn == 0 ? PLAYER1 : PLAYER2);
		turn = (turn + 1) % 2;

		winner = chessboard_winner(chessboard);
	} while (winner != PLAYER1 && winner != PLAYER2);

	chessboard->state = winner;

	chessboard_draw(chessboard);
	printf("player%d wins the game!\n", winner);

	return 0;
}
