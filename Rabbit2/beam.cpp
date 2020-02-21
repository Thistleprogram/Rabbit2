#include"beam.hpp"

Beam::Beam(Vec2 beg, Vec2 ed, int from,Color color) :begin(beg), end(ed), start(sw.ms()), time(800), speed(20), width(2), alive(1), from(from),color(color) {
	ini = (end - begin) / (end - begin).length();
	end = begin + ini * 40;
	beam = Line(begin, end);
}

bool Beam::dele() {
	return sw.ms() - start >= time;
}
void Beam::update(ld delta) {
	begin += ini * speed;
	end += ini * speed;
}
void Beam::draw(ld delta) {
	update(delta);
	beam = Line(begin, end);
	if (ppos.x < 600) beam.draw(width, color);
	else if (ppos.x < ssize - 600) beam.movedBy(600 - ppos.x, 0).draw(width, color);
	else beam.movedBy(1200 - ssize, 0).draw(width, color);
}

Array<Beam>beams;
Array<Beam>bbuffer;
Array<Beam>dummy;