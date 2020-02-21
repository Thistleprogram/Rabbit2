#include "mouse.hpp"

ll clicking[20];
ll rolling[20];

Vec2 Cpos(bool l = 0, int event = 0) {
	if (l == 0) {
		return Cursor::Pos();
	}
	else {

	}
}
bool isclicked(bool l = 0, int num = 0, int event = 0) {
	if (!l) {
		if (num == 0 && MouseL.pressed()) {
			return 1;
		}
		else if (num == 1 && MouseR.pressed() && sw.ms() - clicking[event] > 200) {
			clicking[event] = sw.ms();
			return 1;
		}
		return 0;
	}
	else {

	}
	return 1;
}
bool isrolled(bool l = 0, int event = 0) {
	if (!l) {
		if (sw.ms() - rolling[event] > 200 && Mouse::Wheel()) {
			return 1;
			rolling[event] = sw.ms();
		}
		return 0;
	}
	return 1;
}