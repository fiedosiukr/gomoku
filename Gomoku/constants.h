#include <vector>

enum class PieceType {
	BLACK,
	WHITE
};

static const std::vector<std::pair<int, int>> OFFSETS = 
	{ {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, 
	{1, 0}, {-1, 1}, {0, 1}, {1, 1}};