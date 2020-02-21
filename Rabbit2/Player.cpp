#include "player.hpp"

Player::Player(Vec2 pos, String path) :pos(pos), speed(Vec2{ 0,0 }), accel(Vec2(0, 0)), riding(0), ltouch(0), rtouch(0), hp(1000), len(300),
anchor(Vec2{ 0,0 }), isinjected(0), img_l(Texture(path)), img_r(Image(path).mirror()), img_x(28), img_y(38), shield(0), eval(0), altime(1000), unfolded(0) {
	img = img_l;
}

void Player::touch(Vec2& sum, ld delta) {
	for (const auto& block : blocks[scene]) {
		bool k = 0;
		Rect wall = block.block;
		if (block.hp <= 0) continue;
		if (!prec.intersects(wall)) continue;
		if (pos.x < wall.center().x && pos.y < wall.center().y) {
			if (prec.br().y - wall.tl().y > prec.br().x - wall.tl().x) {
				pos.x = wall.tl().x - img_x/2;
				speed.x = 0; sum.x = 0;
				k = 1;
			}
			else {
				speed.y = 0; sum.y = 0;
				pos.y = wall.tl().y - img_y/2;
				k = 1;
			}
		}
		else if (pos.x < wall.center().x && pos.y >= wall.center().y) {
			if (wall.bl().y - prec.tr().y > prec.tr().x - wall.bl().x) {
				pos.x = wall.bl().x - img_x/2;
				speed.x = 0; sum.x = 0;
				k = 1;
			}
			else {
				speed.y = 0; sum.y = 0;
				pos.y = wall.bl().y + img_y/2;
				k = 1;
			}
		}
		else if (pos.x > wall.center().x&& pos.y < wall.center().y) {
			if (prec.bl().y - wall.tr().y > wall.tr().x - prec.bl().x) {
				pos.x = wall.tr().x + img_x/2;
				speed.x = 0; sum.x = 0;
				k = 1;
			}
			else {
				pos.y = wall.tr().y - img_y/2;
				speed.y = 0; sum.y = 0;
				k = 1;
			}
		}
		else {
			if (wall.br().y - prec.tl().y > wall.br().x - prec.tl().x) {
				pos.x = wall.br().x + img_x/2;
				speed.x = 0; sum.x = 0;
				k = 1;
			}
			else {
				pos.y = wall.br().y + img_y/2;
				speed.y = 0; sum.y = 0;
				k = 1;
			}
		}
		if (k && block.danger) {
			hp -= delta * 500;
		}
	}
}
void Player::touching() {
	for (const auto wall : blocks[scene]) {
		if (wall.hp <= 0) continue;
		if (wall.block.tl().y == pos.y + img_y / 2) riding = 1;
		if (wall.block.tl().x == pos.x + img_x / 2) rtouch = 1;
		if (wall.block.tr().x == pos.x - img_x / 2) ltouch = 1;
	}
}
void Player::update(ld delta) {
	prec = Rect(pos.x - img_x / 2, pos.y - img_y / 2, img_x, img_y);
	Vec2 sum(0, 0);
	sum += gravity * delta;
	riding = 0; ltouch = 0; rtouch = 0;
	Vec2 cpos = Cpos(0, 0);
	if (pos.x < 600);
	else if (pos.x < ssize - 600) cpos.x += (pos.x - 600);
	else cpos.x += ssize - 1200;
	touching();
	touching();
	bool isw = 0;
	if (KeyW.pressed()) {
		if (riding) sum.y -= 450;
		else if (speed.y > -300) sum.y -= (300 + speed.y) * delta;
	}
	if (KeyA.pressed()) {
		//座標をマイナスする
		if (rtouch) {
			sum.x -= 300;
			sum.y -= 400;
		}
		else if (speed.x > -300) sum.x -= (300 + speed.x) * delta;
		isw = 1;
	}
	if (KeyD.pressed()) {
		if (ltouch) {
			sum.x += 300;
			sum.y -= 400;
		}
		else if (speed.x < 300) sum.x += (300 - speed.x) * delta;
		isw = 1;
	}
	if (riding && !isw) {
		speed.x *= 0.9;
	}

	if (isclicked(0, 0, 0)) {
		if (!isinjected) {
			Line rope(pos, cpos);
			rope.end = rope.begin + (rope.end - pos) / Vec2(pos.x - rope.end.x, pos.y - rope.end.y).length() * len;
			Vec2 pt(-1, -1);
			ld dist = inf;
			int tmp = 0;
			for (const auto block : blocks[scene]) {
				if (block.hp <= 0) continue;
				if (const auto points = block.block.intersectsAt(rope)) {
					for (const auto& point : points.value()) {
						ld tt = Vec2(point.x - pos.x, point.y - pos.y).length();
						if (dist > tt) {
							pt = point;
							dist = tt;
						}
					}
				}
			}
			if (pt != Vec2{ -1, -1 }) {
				anchor = pt;
				isinjected = 1;
			}
		}
	}
	else {
		isinjected = 0;
	}
	if (isinjected) {
		Vec2 tmp = Vec2(anchor.x - pos.x, anchor.y - pos.y) / 5;
		if (tmp.length() <= 20) tmp *= 20 / tmp.length();
		sum += tmp;
		rope = Line(pos, anchor);
	}

	if (isclicked(0, 1, 0)) {
		beams << Beam(pos, cpos, 0, Palette::Aqua);
	}
	if (isrolled(0, 0)) {
		slashes << Slash(pos, atan2(Cursor::Pos().y - adj(pos).y, Cursor::Pos().x - adj(pos).x) - PI / 12 + PI / 2, 0, 50, 0);
		slashes << Slash(pos, atan2(Cursor::Pos().y - adj(pos).y, Cursor::Pos().x - adj(pos).x) + PI / 12 + PI / 2, 1, 50, 0);
	}
	bool preshi = shield;
	if (sw.ms() - unfolded >= eval && KeyS.pressed()) {
		shield = 1;
	}
	if (!preshi && shield) {
		eval += 1000;
		altime = 3000;
	}
	else if (shield) {
		altime -= delta * 1000;
		if (altime <= 0) shield = 0;
		unfolded = sw.ms();
	}
	speed += sum;
	pos += speed * delta;
	prec = Rect(pos.x - img_x / 2, pos.y - img_y / 2, img_x, img_y);
	touching();
	touching();
	if (ltouch | rtouch) {
		speed.y = std::min(speed.y, (double)100);
	}
	touch(sum, delta);
	touch(sum, delta);
	prec = Rect(pos.x - img_x / 2, pos.y - img_y / 2, img_x, img_y);
	if (sum.x > 0) img = img_r;
	else if (sum.x < 0) img = img_l;
	ppos = pos;
}

void Player::draw(ld delta) {
	update(delta);
	Circle(adj(pos), len).draw(Color(175, 255, 149, 100));
	img.drawAt(adj(pos));
	if (shield) {
		Circle(adj(pos), 40).draw(Color(0, 0, 255, 150));
		Shield = Circle(pos, 40);
	}
	if (isinjected) Line(adj(pos), adj(anchor)).draw(Palette::Black);
	hp = std::max(hp, 0);
}
void Player::hit(Beam& beam) {
	if (beam.from == 1) {
		beam.alive = 0;
		if (!shield) {
			hp -= 10;
		}
	}
}