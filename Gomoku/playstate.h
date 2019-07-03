#pragma once
#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "piece.h"


class PlayState {
public:
	PlayState();
	~PlayState();
	void update();
	void render();
	void check_events(ALLEGRO_EVENT event);
private:
	ALLEGRO_FONT* small_font;
	ALLEGRO_FONT* big_font;

	std::pair<int, int> mouse;

	Piece* pieces[BOARD_SIZE][BOARD_SIZE];
	void place_piece(int x, int y);

	bool game_over;

	PieceType active_player;
	void change_player();
	bool is_on_board(int i, int j);
	bool check_win_condition(int i, int j);
	bool checking_loop(int i, int j, std::pair<int, int> offset, int* num_pieces);
	void increment_pieces();
	void restart_game();

	int num_whites;
	int num_blacks;
};

