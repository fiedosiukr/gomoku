#include "playstate.h"

#include <allegro5/allegro_primitives.h>
#include <string>

#include "constants.h"

PlayState::PlayState() {
	for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			pieces[i][j] = nullptr;

	active_player = PieceType::BLACK;
	game_over = false;

	num_whites = num_blacks = 0;

	small_font = al_load_font("arial.ttf", SMALL_FONT_SIZE, 0);
	big_font = al_load_font("arial.ttf", BIG_FONT_SIZE, 0);
}

PlayState::~PlayState() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (pieces[i][j] != nullptr)
				delete pieces[i][j];
		}
	}

	al_destroy_font(small_font);
	al_destroy_font(big_font);
}

void PlayState::render() {
	al_clear_to_color(al_map_rgb(230, 186, 99));

	for (int i = 0; i < BOARD_SIZE - 1; i++) {
		for (int j = 0; j < BOARD_SIZE - 1; j++) {
			al_draw_rectangle(i*CELL_SIZE + BOARD_OFFSET, 
				j*CELL_SIZE + BOARD_OFFSET, 
				(i+1) * CELL_SIZE + BOARD_OFFSET, 
				(j+1) * CELL_SIZE + BOARD_OFFSET,
				al_map_rgb(0, 0, 0), 1);
		}
	}

	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (pieces[i][j] == nullptr)
				continue;

			ALLEGRO_COLOR color = pieces[i][j]->get_type() == PieceType::BLACK ? al_map_rgb(0, 0, 0) : al_map_rgb(255, 255, 255);
			al_draw_filled_circle(i * CELL_SIZE + BOARD_OFFSET, j * CELL_SIZE + BOARD_OFFSET, PIECE_RADIUS, color);
		}
	}

	std::string player;
	if (active_player == PieceType::WHITE)
		player = "White is on the move.";
	else
		player = "Black is on the move.";

	std::string whites = "Number of whites: " + std::to_string(num_whites);
	std::string blacks = "Number of blacks: " + std::to_string(num_blacks);

	al_draw_text(small_font, al_map_rgb(0, 0, 0), TEXT_OFFSET, BOARD_OFFSET + SMALL_FONT_SIZE, ALLEGRO_ALIGN_CENTER, player.c_str());
	al_draw_text(small_font, al_map_rgb(0, 0, 0), TEXT_OFFSET, BOARD_OFFSET + 4 * SMALL_FONT_SIZE, ALLEGRO_ALIGN_CENTER, whites.c_str());
	al_draw_text(small_font, al_map_rgb(0, 0, 0), TEXT_OFFSET, BOARD_OFFSET + 5.5 * SMALL_FONT_SIZE, ALLEGRO_ALIGN_CENTER, blacks.c_str());

	if (!game_over) {
		if (mouse.first >= BOARD_OFFSET - CELL_SIZE / 2 &&
			mouse.second >= BOARD_OFFSET - CELL_SIZE / 2 &&
			mouse.first < BOARD_SIZE * CELL_SIZE + BOARD_OFFSET - CELL_SIZE / 2 &&
			mouse.second < BOARD_SIZE * CELL_SIZE + BOARD_OFFSET - CELL_SIZE / 2) {

			int i = (mouse.first - (BOARD_OFFSET - CELL_SIZE / 2)) / CELL_SIZE;
			int j = (mouse.second - (BOARD_OFFSET - CELL_SIZE / 2)) / CELL_SIZE;

			ALLEGRO_COLOR color = active_player == PieceType::BLACK ? al_map_rgb(0, 0, 0) : al_map_rgb(255, 255, 255);

			if (pieces[i][j] == nullptr)
				al_draw_filled_circle(i * CELL_SIZE + BOARD_OFFSET, j * CELL_SIZE + BOARD_OFFSET, PIECE_RADIUS, color);
		}
	} else {
		std::string winner;

		if (active_player == PieceType::WHITE)
			winner = "White is the winner!";
		else
			winner = "Black is the winner!";
		al_draw_text(big_font, al_map_rgb(184, 11, 63), WIDTH / 2, HEIGHT / 2 - BIG_FONT_SIZE, ALLEGRO_ALIGN_CENTER, winner.c_str());
	}
}

void PlayState::update() {
	if (game_over)
		std::cout << " XDDDDDDDDD";

}

void PlayState::check_events(ALLEGRO_EVENT event) {
	if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY)
		mouse = std::make_pair(event.mouse.x, event.mouse.y);
	else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && event.mouse.button == 1) {
		if (!game_over)
			place_piece(mouse.first, mouse.second);
		else
			restart_game();
	}
}

void PlayState::restart_game() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (pieces[i][j] != nullptr) {
				delete pieces[i][j];
				pieces[i][j] = nullptr;
			}
		}
	}

	num_blacks = num_whites = 0;
	game_over = false;
	active_player = PieceType::BLACK;
}

void PlayState::place_piece(int x, int y) {
	if (x >= BOARD_OFFSET - CELL_SIZE / 2 &&
		y >= BOARD_OFFSET - CELL_SIZE / 2 &&
		x < BOARD_SIZE * CELL_SIZE + BOARD_OFFSET - CELL_SIZE / 2 &&
		y < BOARD_SIZE * CELL_SIZE + BOARD_OFFSET - CELL_SIZE / 2) {

		int i = (x - (BOARD_OFFSET - CELL_SIZE / 2)) / CELL_SIZE;
		int j = (y - (BOARD_OFFSET - CELL_SIZE / 2)) / CELL_SIZE;
		if (pieces[i][j] == nullptr) {
			pieces[i][j] = new Piece(active_player);
			increment_pieces();
			if (!check_win_condition(i, j))
				change_player();
			else
				game_over = true;
		}
	}
}

void PlayState::change_player() {
	active_player = active_player == PieceType::BLACK ? PieceType::WHITE : PieceType::BLACK;
}

void PlayState::increment_pieces() {
	active_player == PieceType::BLACK ? num_blacks++ : num_whites++;
}

bool PlayState::check_win_condition(int i, int j) {
	for (auto offset : OFFSETS) {
		int num_pieces = 1;
		if (checking_loop(i, j, offset, &num_pieces))
			return true;
		
		std::pair<int, int> reverse_offset = std::make_pair(offset.first * -1, offset.second * -1);
		if (checking_loop(i, j, reverse_offset, &num_pieces))
			return true;
	}

	return false;
}

bool PlayState::checking_loop(int i, int j, std::pair<int, int> offset, int* num_pieces) {
	int current_x = i + offset.first;
	int current_y = j + offset.second;

	while (is_on_board(current_x, current_y)) {
		if (pieces[current_x][current_y] == nullptr) {
			if (*num_pieces == NUMBER_IN_ROW)
				return true;
			else
				break;
		}
		else if (pieces[current_x][current_y]->get_type() == active_player) {
			current_x += offset.first;
			current_y += offset.second;
			(*num_pieces)++;
		}
		else {
			if (*num_pieces == NUMBER_IN_ROW)
				return true;
			else
				break;
		}
	}

	return false;
}

bool PlayState::is_on_board(int i, int j) {
	return (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE);
}
