#include "piece.h"


Piece::Piece(PieceType type) {
	this->type = type;
}

PieceType Piece::get_type() {
	return type;
}
