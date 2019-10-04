#include "Color.h"
#include <cmath>

hf::Color::Color() : Color(0.f){}

hf::Color::Color(const float grayscale, const float a) : Color(grayscale, grayscale, grayscale, a){}

hf::Color::Color(const float r, const float g, const float b, const float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

bool hf::Color::is_similar_to(const Color& other, const float threshold) const{
    float r = this->r - other.r;
    float g = this->g - other.g;
    float b = this->b - other.b;
    float a = this->a - other.a;

    return (r * r + g * g + b * b + a * a) < (threshold * threshold);
}