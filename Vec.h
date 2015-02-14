#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <array>
#include <stdint.h>

template <typename T, size_t N>
struct Vec {
    std::array<T, N> v;

    Vec<T, N>() {
    }

    Vec<T, N>(const Vec<T, N>& other) {
        v = other.v;
    }

    ~Vec<T, N>() {
    }

    auto magnitude() -> decltype(sqrt(v[0])) const {
        return sqrt(magnitude_square());
    }

    T magnitude_square() const {
        T sum = T();
        for (size_t i = 0; i < N; ++i) {
            sum += v[i] * v[i];
        }
        return sum;
    }

    void zero() {
        for (size_t i = 0; i < N; ++i) {
            v[i] = T();
        }
    }

    void normalize() {
        *this /= magnitude();
    }

    Vec<T, N>& operator+=(const Vec<T, N>& rhs) {
        for (size_t i = 0; i < N; ++i) {
            v[i] += rhs[i];
        }
        return *this;
    }

    Vec<T, N>& operator-=(const Vec<T, N>& rhs) {
        for (size_t i = 0; i < N; ++i) {
            v[i] -= rhs[i];
        }
        return *this;
    }

    Vec<T, N>& operator*=(T factor) {
        for (size_t i = 0; i < N; ++i) {
            v[i] *= factor;
        }
        return *this;
    }

    Vec<T, N>& operator/=(T factor) {
        for (size_t i = 0; i < N; ++i) {
            v[i] /= factor;
        }
        return *this;
    }

    Vec<T, N>& operator=(const Vec<T, N>& rhs) {
        v = rhs.v;
        return *this;
    }

    Vec<T, N> operator+(const Vec<T, N>& rhs) const {
        Vec<T, N> new_vec;
        for (size_t i = 0; i < N; ++i) {
            new_vec[i] = v[i] + rhs[i];
        }
        return new_vec;
    }

    Vec<T, N> operator-(const Vec<T, N>& rhs) const {
        Vec<T, N> new_vec;
        for (size_t i = 0; i < N; ++i) {
            new_vec[i] = v[i] - rhs[i];
        }
        return new_vec;
    }

    Vec<T, N> operator*(T factor) const {
        Vec<T, N> new_vec;
        for (size_t i = 0; i < N; ++i) {
            new_vec[i] = v[i] * factor;
        }
        return new_vec;
    }

    Vec<T, N> operator/(T factor) const {
        Vec<T, N> new_vec;
        for (size_t i = 0; i < N; ++i) {
            new_vec[i] = v[i] / factor;
        }
        return new_vec;
    }

    T& operator[](size_t index) {
        return v[index];
    }

    T operator[](size_t index) const {
        return v[index];
    }

    bool operator==(const Vec<T, N>& rhs) const {
        return v == rhs.v;
    }

    bool operator!=(const Vec<T, N>& rhs) const {
        return v != rhs.v;
    }

    T dot(const Vec<T, N>& other) const {
        T sum = T();
        for (size_t i = 0; i < N; ++i) {
            sum += v[i] * other[i];
        }
        return sum;
    }
};

/* This needed the extra template parameter to get the compiler to stop
 * producing errors on the potentially mismatched types of the Vector
 * components and factor */
template <typename T, typename U, size_t N>
Vec<T, N> operator*(U f, const Vec<T, N>& rhs) {
    T factor = T(f);
    Vec<T, N> new_vec;
    for (size_t i = 0; i < N; ++i) {
        new_vec[i] = factor * rhs[i];
    }
    return new_vec;
}


// It's a tad bit hacky
template <typename T>
struct Vec2: public Vec<T, 2> {

    Vec2<T>() {
    }

    Vec2<T>(T angle) {
        x() = cos(angle);
        y() = sin(angle);
    }

    Vec2<T>(T _x, T _y) {
        x() = _x;
        y() = _y;
    }

    Vec2<T>(Vec<T, 2> rhs) {
        x() = rhs[0];
        y() = rhs[1];
    }

    T& x() {
        return (*this)[0];
    }

    T x() const {
        return (*this)[0];
    }

    T& y() {
        return (*this)[1];
    }

    T y() const {
        return (*this)[1];
    }

    auto theta() -> decltype(atan2(x(), y())) const {
        return atan2(y(), x());
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;
typedef Vec2<int8_t> Vec2i8;
typedef Vec2<int32_t> Vec2i32;
typedef Vec2<int64_t> Vec2i64;
typedef Vec2<uint8_t> Vec2u8;
typedef Vec2<uint32_t> Vec2u32;
typedef Vec2<uint64_t> Vec2u64;

// ^^
template <typename T>
struct Vec3: public Vec<T, 3> {

    Vec3<T>() {
    }

    Vec3<T>(T _x, T _y, T _z) {
        x() = _x;
        y() = _y;
        z() = _z;
    }

    Vec3<T>(Vec<T, 3> rhs) {
        x() = rhs[0];
        y() = rhs[1];
        z() = rhs[2];
    }

    T& x() {
        return (*this)[0];
    }

    T x() const {
        return (*this)[0];
    }

    T& y() {
        return (*this)[1];
    }

    T y() const {
        return (*this)[1];
    }

    T& z() {
        return (*this)[2];
    }

    T z() const {
        return (*this)[2];
    }

    T pitch() const {
        return 0.0;
    }

    T roll() const {
        return 0.0;
    }

    T yaw() const {
        return 0.0;
    }

    Vec3<T> cross(const Vec3<T>& other) const {
        T _x = y() * other.z() - z() * other.y();
        T _y = z() * other.x() - x() * other.z();
        T _z = x() * other.y() - y() * other.x();
        return Vec3<T>(_x, _y, _z);
    }
};


typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<int8_t> Vec3i8;
typedef Vec3<int32_t> Vec3i32;
typedef Vec3<int64_t> Vec3i64;
typedef Vec3<uint8_t> Vec3u8;
typedef Vec3<uint32_t> Vec3u32;
typedef Vec3<uint64_t> Vec3u64;

#endif

