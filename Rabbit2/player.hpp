#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "base.hpp"
#include "mouse.hpp"
#include "block.hpp"
#include "beam.hpp"
#include "slash.hpp"

struct Player {
	//��{�f�[�^
	const Texture img_l, img_r;
	Texture img;
	const int img_x, img_y;
	Vec2 pos, speed, accel;
	Rect prec;
	bool riding, ltouch, rtouch;
	int hp;
	int eval, altime;
	//���݂̍����A�c��V�[���h�W�J�\����
	int unfolded;//�O��̓W�J����
	bool shield;
	Circle Shield;

	//���C���[�f�[�^
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