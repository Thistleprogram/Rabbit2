#include "trophy.hpp"
Trophy::Trophy(Vec2 vec) :img(Texture(U"Datas/trophy.png")), trophy(vec.x, vec.y, 90, 100), cleared(0) {}
void Trophy::draw(Player player) {
	img.draw(adj(trophy.tl()));
	if (player.prec.intersects(trophy)) {
		cleared = 1;
	}
}