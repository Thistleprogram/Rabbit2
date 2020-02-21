#include "slash.hpp"

Slash::Slash(Vec2 mid, ld start, bool l, int size, bool from) : mid(mid), start(start), l(l),
		touch(0), alive(1), from(from) {
	stime = sw.ms();
	eval = 0;
	slash = Circle(mid, size);
}
void Slash::update(ld delta) {
	slash.x = ppos.x;
	slash.y = ppos.y;
	if (!touch) {
		if (l) eval += 5 * PI * delta;
		else eval -= 5 * PI * delta;
	}//Ç‹Ç∏ÇÕãóó£0Ç©ÇÁénÇﬂÇÈÅB
	else {
		if (l) {
			eval -= 10 * PI * delta;
			start += 10 * PI * delta;
			if (eval < 0) alive = 0;
		}
		else {
			eval += 10 * PI * delta;
			start -= 10 * PI * delta;
			if (eval > 0) alive = 0;
		}
	}
	if (abs(eval) > 2 * PI / 3) touch = 1;
}
void Slash::draw(ld delta) {
	update(delta);
	slash.x = adj(ppos).x;
	slash.drawPie(start, eval, Palette::Aqua);
	slash.x = ppos.x;
}

Array<Slash>slashes;