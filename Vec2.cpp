#include <cmath>
#include "Vec2.h"

static const float PI = 3.14159265358979323846264338327950288419716939937510;

Vec2::Vec2() :
    x(0.0),
    y(0.0) {
}

Vec2::Vec2(float x, float y): 
    x(x),
    y(y) {
}

Vec2::~Vec2() {
}

float Vec2::magnitude() {
    return sqrt(x * x + y * y);
}
float Vec2::magnitude_square() {
    return x * x + y * y;
}

float Vec2::angle() {
    if (x > 0.0) {
        if (y >= 0.0) {
            return atan(y / x);
        } else {
            return atan(y / x) + 2.0 * PI;
        }
    }
    else if (x < 0.0) {
        return atan(y / x) + PI;
    } else if (x == 0.0) {
        if (y > 0.0) {
            return PI / 2.0;
        } else {
            return 3.0 * PI / 2.0;
        }
    }
    else {
        // The x and y coordinates are the same
        return nan("");
    }
}

void Vec2::zero() {
    x = 0.0;
    y = 0.0;
}

void Vec2::normalize() {
    *this /= magnitude();
}

Vec2& Vec2::operator+=(const Vec2& rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vec2& Vec2::operator*=(const float factor) {
    x *= factor;
    y *= factor;
    return *this;
}

Vec2& Vec2::operator/=(const float factor) {
    x /= factor;
    y /= factor;
    return *this;
}

Vec2& Vec2::operator=(const Vec2& rhs) {
    x = rhs.x;
    y = rhs.y;
    return *this;
}

Vec2 Vec2::operator+(const Vec2& rhs) {
    return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) {
    return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator*(const float factor) {
    return Vec2(x * factor, y * factor);
}

Vec2 Vec2::operator/(const float factor) {
    return Vec2(x / factor, y / factor);
}

