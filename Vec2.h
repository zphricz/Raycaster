#ifndef VEC2_H
#define VEC2_H

class Vec2 {
    public:
        float x;
        float y;

        Vec2();
        Vec2(float x, float y);
        ~Vec2();
        
        float magnitude();
        float magnitude_square();
        float angle();
        void zero();
        void normalize();

        Vec2& operator+=(const Vec2& rhs);
        Vec2& operator-=(const Vec2& rhs);
        Vec2& operator*=(const float factor);
        Vec2& operator/=(const float factor);
        Vec2& operator=(const Vec2& rhs);
        Vec2 operator+(const Vec2& rhs);
        Vec2 operator-(const Vec2& rhs);
        Vec2 operator*(const float factor);
        Vec2 operator/(const float factor);
};

#endif
