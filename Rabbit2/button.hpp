#include "base.hpp"

struct Button {
	Rect button;
	Font font;
	Color color;
	String str;
	bool valid;
	Button(Rect button, Color color, int ft, String str, bool valid);
	void draw();
	bool onclick();
};