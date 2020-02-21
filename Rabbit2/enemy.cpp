#include "enemy.hpp"

Enemy::Enemy(Vec2 enem, bool refl, int nume, int hp, int res = 0) :type(nume), eval(RandomVec2(Circle(0, 0, 100)).x + 500), res(res), three(0), hp(hp), refl(refl) {
	if (refl) {
		img = Texture(U"Datas/enemy2.png");
	}
	else {
		img = Texture(U"Datas/enemy.png");
	}
	if (type == 3) {
		three = enem.y;
		enem.y = three + Random(4) * (res - three) / 5;
	}
	if (type == 0 && res == 1) {
		eval = RandomVec2(Circle(0, 0, 100)).x;
	}
	enemy = Rect(enem.x, enem.y, 30, 30);
}
void Enemy::update(ld delta) {
	if (std::abs(ppos.x - enemy.center().x) < 1200) {
		if (sw.ms() - pre > eval) {
			if (type == 0) {
				if (refl) beams << Beam(enemy.center(), Vec2(enemy.center().x + (ppos.x < enemy.center().x ? -1 : 1), enemy.center().y), 1, Palette::Blue);
				else beams << Beam(enemy.center(), Vec2(enemy.center().x + (ppos.x < enemy.center().x ? -1 : 1), enemy.center().y), 1, Palette::Red);
			}
			else {
				if (type == 3) {
					for (int i = 0; i < 5; i++) {
						if ((three + (res - three) / 5 * i) == enemy.y) {
							if (refl)beams << Beam(Vec2(enemy.center().x, three + (res - three) / 5 * i), ppos, 1, Palette::Blue);
							else beams << Beam(Vec2(enemy.center().x, three + (res - three) / 5 * i), ppos, 1, Palette::Red);
						}
						else {
							if (refl) dummy << Beam(Vec2(enemy.center().x, three + (res - three) / 5 * i), ppos, 1, Palette::Blue);
							else dummy << Beam(Vec2(enemy.center().x, three + (res - three) / 5 * i), ppos, 1, Palette::Red);
						}
					}
				}
				else {
					if(refl) beams << Beam(enemy.center(), ppos, 1, Palette::Blue);
					else beams << Beam(enemy.center(), ppos, 1, Palette::Red);
				}
			}
			pre = sw.ms();
		}
		if (type == 2) {
			if (!enemy.contains(ppos)) {
				enemy = enemy.moveBy(((ppos - enemy.center()).normalize() * res * delta).x, ((ppos - enemy.center()).normalize() * res * delta).y);
			}
		}
	}
}
void Enemy::draw(ld delta) {
	if (abs(ppos.x - enemy.x) > 1000) return;
	if (hp <= 0) return;
	update(delta);
	if (type == 3) {
		for (int i = 0; i < 5; i++)
			img.drawAt(adj(Vec2(enemy.center().x, three + (res - three) / 5 * i)));
	}
	else img.drawAt(adj(enemy.center()));
}
void Enemy::hit(Beam& beam) {
	if (refl) {
		ld r = atan2(beam.end.y - enemy.center().y, beam.end.x - enemy.center().x);
		bbuffer << Beam(enemy.center(), beam.begin, 1, Palette::Blue);
		bbuffer << Beam(enemy.center(), OffsetCircular(enemy.center(), 40, r - 1), 1, Palette::Blue);
		bbuffer << Beam(enemy.center(), OffsetCircular(enemy.center(), 40, r + 1), 1, Palette::Blue);
		beam.alive = 0;
	}
	else if (beam.from == 0) {
		hp -= 30;
		beam.alive = 0;
	}
}


Array<Enemy>enemies[10];