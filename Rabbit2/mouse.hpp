#ifndef MOUSE_HPP
#define MOUSE_HPP

#include "base.hpp"

extern Vec2 Cpos(bool l, int event);
extern bool isclicked(bool l, int num, int event);
extern bool isrolled(bool l, int event);

#endif