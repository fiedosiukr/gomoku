#pragma once
#include <vector>


static const int BOARD_SIZE = 15;

static const int WIDTH = 800;
static const int HEIGHT = 600;

static const int BOARD_OFFSET = 48;
static const int CELL_SIZE = 36;
static const int PIECE_RADIUS = 16;

static const int NUMBER_IN_ROW = 5;

static const int SMALL_FONT_SIZE = 18;
static const int BIG_FONT_SIZE = 35;

static const int TEXT_OFFSET = 700;

static const double FPS = 60.0;

enum class PieceType {
	BLACK,
	WHITE
};

static const std::vector<std::pair<int, int>> OFFSETS = 
	{ {-1, -1}, {0, -1}, {1, -1}, {-1, 0}};