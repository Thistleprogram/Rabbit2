#ifndef BEAM_HPP
#define BEAM_HPP

#include"base.hpp"

struct Beam {
	Vec2 begin, end;
	Vec2 ini;
	Line beam;
	int width;
	ld speed;
	int time;
	int start;
	int from;
	//0=player
	//1=enemy
	bool alive;
	Color color;
	Beam(Vec2 beg, Vec2 ed, int from, Color color);
	bool dele();
	void update(ld delta);
	void draw(ld delta);
};

extern Array<Beam>beams;
extern Array<Beam>bbuffer;
extern Array<Beam>dummy;

#endif