#include "block.hpp"

Block::Block(Rect block, int hp, bool danger, bool type, int c00, int c10 = 0) :block(block), hp(hp), danger(danger), type(type) {
	Color colors[11] = { Palette::Gray,Palette::Black,Palette::Green,Palette::Brown,
	Palette::Red,Palette::Yellow,Palette::Lime,Palette::Maroon,Palette::Sienna,Palette::Blue,Palette::White };
	for (int i = 0; i < 11; i++) {
		if (c00 == i) c0 = colors[i];
		if (c10 == i) c1 = colors[i];
	}
}
void Block::hit(double magi) {
	hp -= 30 * magi;
}
void Block::draw() {
	if (hp <= 0) return;
	if (type) {
		for (int i = 0; i < block.w / 20; i++) {
			for (int j = 0; j < block.h / 20; j++) {
				Rect(adj(Vec2(block.x + i * 20, 0)).x, block.y + j * 20, 20, 20).draw(((i + j) % 2 == 0 ? c0 : c1));
			}
		}
	}
	else {
		Rect(adj(block.tl()).x, block.tl().y, block.size).draw(c0);
	}
}

Array<Block>blocks[10];