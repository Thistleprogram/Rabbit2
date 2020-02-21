#include "base.hpp"
#include "beam.hpp"

struct Block {
	Rect block;
	int hp;
	bool danger, type;
	Color c0, c1;
	Block(Rect block, int hp,bool danger, bool type, int c00, int c10);
	void hit(double magi = 1);
	void draw();
};
extern Array<Block>blocks[10];