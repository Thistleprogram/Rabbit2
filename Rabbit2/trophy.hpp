#include "base.hpp"
#include "player.hpp"
struct Trophy {
	Texture img;
	Rect trophy;
	bool cleared;
	Trophy(Vec2 vec);
	void draw(Player player);
};