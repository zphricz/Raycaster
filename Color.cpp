#include <algorithm>
#include "Color.h"

using namespace std;

Color::Color() {
}

Color::Color(Uint8 r, Uint8 g, Uint8 b) :
    r(r),
    g(g),
    b(b),
    a(255) {
}

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) :
    r(r),
    g(g),
    b(b),
    a(a) {
}

Color Color::operator*(float factor) const {
    Uint8 out_r = min((int)(r * factor), 255);
    Uint8 out_g = min((int)(g * factor), 255);
    Uint8 out_b = min((int)(b * factor), 255);
    return {out_r, out_g, out_b, a};
}

Color Color::operator/(float factor) const {
    return {Uint8(r / factor), Uint8(g / factor), Uint8(b / factor), a};
}

Color& Color::operator*=(float factor) {
    return *this = *this * factor;
}

Color& Color::operator/=(float factor) {
    return *this = *this / factor;
}

Color blend(const Color& fg, const Color& bg) {
    unsigned int out_a = fg.a + bg.a * (256 - fg.a) / 256;
    if (out_a == 0) {
        return {0, 0, 0, 0};
    }
    unsigned int out_r = (fg.r * fg.a + bg.r * bg.a * (255 - fg.a) / 256) / out_a;
    unsigned int out_g = (fg.g * fg.a + bg.g * bg.a * (255 - fg.a) / 256) / out_a;
    unsigned int out_b = (fg.b * fg.a + bg.b * bg.a * (255 - fg.a) / 256) / out_a;
    return {Uint8(out_r), Uint8(out_g), Uint8(out_b), Uint8(out_a)};
}

