#ifndef SLASH_HPP
#define SLASH_HPP

#include "base.hpp"

struct Slash {
	Vec2 mid;
	ld start;
	ld stime, eval;
	bool l, touch;
	Circle slash;
	bool alive;
	bool from;
	Slash(Vec2 mid, ld start,  bool l, int size, bool from);
	//���S�_�A�J�n�p�A���v���A�����v���A���a
	void update(ld delta);
	void draw(ld delta);
};

extern Array<Slash>slashes;

#endif