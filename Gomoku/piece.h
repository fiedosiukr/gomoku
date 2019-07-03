#pragma once
#include "constants.h"

class Piece {
public:
	Piece(PieceType type);

	PieceType get_type();
private:
	PieceType type;
};

