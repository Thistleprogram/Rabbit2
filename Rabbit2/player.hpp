#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "base.hpp"
#include "mouse.hpp"
#include "block.hpp"
#include "beam.hpp"
#include "slash.hpp"

struct Player {
	//基本データ
	const Texture img_l, img_r;
	Texture img;
	const int img_x, img_y;
	Vec2 pos, speed, accel;
	Rect prec;
	bool riding, ltouch, rtouch;
	int hp;
	int eval, altime;
	//現在の差分、残りシールド展開可能時間
	int unfolded;//前回の展開時間
	bool shield;
	Circle Shield;

	//ワイヤーデータ
	Vec2 anchor;
	bool isinjected;
	Line rope;
	const int len;

	Player(Vec2 pos, String path);

	void touch(Vec2& sum, ld delta);
	void touching();
	void update(ld delta);
	void draw(ld delta);
	void hit(Beam& beam);
};

#endif