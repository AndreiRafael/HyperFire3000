#include "Vec2.h"

hf::Vec2::Vec2() : Vec2(0.f){}

hf::Vec2::Vec2(const float xy) : Vec2(xy, xy){}

hf::Vec2::Vec2(const float x, const float y) {
    this->x = x;
    this->y = y;
}