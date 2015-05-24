#ifndef VEC_H
#define VEC_H

#include <cmath>
#include <array>
#include <iomanip>
#include <stdint.h>
#include <sstream>

/*template <typename T>
constexpr pi<T> = T(M_PI);*/

namespace Linear {

template <typename T, size_t M, size_t N> class Matrix;

// Default value for C is column vector
template <typename T, size_t N, bool C = true> class Vec {
private:
  std::array<T, N> v;

public:
  Vec() { static_assert(N > 0, "Vector cannot have a zero dimension"); }

  template <typename U> Vec(const Vec<U, N, C> &other) {
    static_assert(N > 0, "Vector cannot have a zero dimension");
    for (size_t i = 0; i < N; ++i) {
      v[i] = other[i];
    }
  }

  template <typename U> Vec(std::initializer_list<U> list) {
    static_assert(N > 0, "Vector cannot have a zero dimension");
    size_t i = 0;
    for (auto it = list.begin(); it != list.end() && i < N; ++it, ++i) {
      v[i] = *it;
    }
  }

  template <size_t M> Vec(const Matrix<T, 1, M> &other) {
    static_assert(M > 0, "Vector cannot have a zero dimension");
    static_assert(N == M, "Mismatched vector/matrix length");
    static_assert(C == false, "Mismatched vector/matrix dimensions");
    for (size_t i = 0; i < M; ++i) {
      v[i] = other[0][i];
    }
  }

  template <size_t M> Vec(const Matrix<T, M, 1> &other) {
    static_assert(M > 0, "Vector cannot have a zero dimension");
    static_assert(N == M, "Mismatched vector/matrix length");
    static_assert(C == true, "Mismatched vector/matrix dimensions");
    for (size_t i = 0; i < M; ++i) {
      v[i] = other[i][0];
    }
  }

  Vec(const Matrix<T, 1, 1> &other) {
    static_assert(N == 1, "Mismatched vector/matrix length");
    v[0] = other[0][0];
  }

  static Vec zeros() {
    static_assert(N > 0, "Vector cannot have a zero dimension");
    Vec<T, N, C> rval;
    rval.zero();
    return rval;
  }

  ~Vec() {}

  decltype(sqrt(T())) magnitude() const {
    return sqrt(magnitude_square());
  }

  T magnitude_square() const {
    T sum = T(0);
    for (size_t i = 0; i < N; ++i) {
      sum += v[i] * v[i];
    }
    return sum;
  }

  void zero() {
    for (size_t i = 0; i < N; ++i) {
      v[i] = T(0);
    }
  }

  Vec normal() { return *this / magnitude(); }

  void normalize() { *this = normal(); }

  Vec &operator+=(const Vec<T, N, C> &rhs) {
    for (size_t i = 0; i < N; ++i) {
      v[i] += rhs[i];
    }
    return *this;
  }

  Vec &operator-=(const Vec<T, N, C> &rhs) {
    for (size_t i = 0; i < N; ++i) {
      v[i] -= rhs[i];
    }
    return *this;
  }

  Vec &operator*=(T factor) {
    for (size_t i = 0; i < N; ++i) {
      v[i] *= factor;
    }
    return *this;
  }

  Vec &operator/=(T factor) {
    for (size_t i = 0; i < N; ++i) {
      v[i] /= factor;
    }
    return *this;
  }

  Vec operator+(const Vec<T, N, C> &rhs) const {
    Vec<T, N, C> new_vec;
    for (size_t i = 0; i < N; ++i) {
      new_vec[i] = v[i] + rhs[i];
    }
    return new_vec;
  }

  Vec operator-(const Vec<T, N, C> &rhs) const {
    Vec<T, N, C> new_vec;
    for (size_t i = 0; i < N; ++i) {
      new_vec[i] = v[i] - rhs[i];
    }
    return new_vec;
  }

  Vec operator*(T factor) const {
    Vec<T, N, C> new_vec;
    for (size_t i = 0; i < N; ++i) {
      new_vec[i] = v[i] * factor;
    }
    return new_vec;
  }

  Vec operator/(T factor) const {
    Vec<T, N, C> new_vec;
    for (size_t i = 0; i < N; ++i) {
      new_vec[i] = v[i] / factor;
    }
    return new_vec;
  }

  T &operator[](size_t index) { return v[index]; }

  T operator[](size_t index) const { return v[index]; }

  bool operator==(const Vec<T, N, C> &rhs) const { return v == rhs.v; }

  bool operator!=(const Vec<T, N, C> &rhs) const { return v != rhs.v; }

  T dot(const Vec<T, N, C> &other) const {
    T sum = T(0);
    for (size_t i = 0; i < N; ++i) {
      sum += v[i] * other[i];
    }
    return sum;
  }

  size_t size() const { return N; }
};

template <typename T, size_t N, bool C>
Vec<T, N, false> to_row(const Vec<T, N, C> &other) {
  Vec<T, N, false> rval;
  static_assert(N > 0, "Vector cannot have a zero dimension");
  for (size_t i = 0; i < N; ++i) {
    rval[i] = other[i];
  }
  return rval;
}

template <typename T, size_t N, bool C>
Vec<T, N> to_col(const Vec<T, N, C> &other) {
  Vec<T, N> rval;
  static_assert(N > 0, "Vector cannot have a zero dimension");
  for (size_t i = 0; i < N; ++i) {
    rval[i] = other[i];
  }
  return rval;
}

/* Prints out the vector as a column vector
 */
template <typename T, size_t N, bool C>
std::ostream &operator<<(std::ostream &os, const Vec<T, N, C> &rhs) {
  if (!C) {
    os << "[ ";
  }
  size_t max_size = 0;
  for (size_t i = 0; i < N; ++i) {
    std::ostringstream s;
    s << +rhs[i];
    std::string str = s.str();
    if (str.size() > max_size) {
      max_size = str.size();
    }
  }
  for (size_t i = 0; i < N; ++i) {
    if (C) {
      os << "[ " << std::setw(max_size) << +rhs[i] << " ]";
      if (i != N - 1) {
        os << std::endl;
      }
    } else {
      os << rhs[i] << " ";
    }
  }
  if (!C) {
    os << "]";
  }
  return os;
}

template <typename T, typename U, size_t N, bool C>
Vec<T, N, C> operator*(U f, const Vec<T, N, C> &rhs) {
  T factor = T(f);
  Vec<T, N, C> new_vec;
  for (size_t i = 0; i < N; ++i) {
    new_vec[i] = factor * rhs[i];
  }
  return new_vec;
}

template <typename T, bool C> class Vec<T, 2, C> {
public:
  T x;
  T y;

  Vec() {}

  template <typename U>
  Vec(const Vec<U, 2, C> &other)
      : x(other.x), y(other.y) {}

  template <typename U> Vec(std::initializer_list<U> list) {
    size_t i = 0;
    for (auto it = list.begin(); it != list.end() && i < 2; ++it, ++i) {
      (*this)[i] = *it;
    }
  }

  Vec(const Matrix<T, 1, 2> &other) : x(other[0][0]), y(other[0][1]) {
    static_assert(C == false, "Mismatched vector/matrix dimensions");
  }

  Vec(const Matrix<T, 2, 1> &other) : x(other[0][0]), y(other[1][0]) {
    static_assert(C == true, "Mismatched vector/matrix dimensions");
  }

  Vec(T angle) : x(cos(angle)), y(sin(angle)) {}

  Vec(T x, T y) : x(x), y(y) {}

  static Vec zeros() {
    Vec<T, 2, C> rval;
    rval.zero();
    return rval;
  }

  ~Vec() {}

  decltype(atan2(T(), T())) theta() const { return atan2(y, x); }

  decltype(sqrt(T())) magnitude() const {
    return sqrt(magnitude_square());
  }

  T magnitude_square() const { return x * x + y * y; }

  void zero() {
    x = T(0);
    y = T(0);
  }

  Vec normal() { return *this / magnitude(); }

  void normalize() { *this = normal(); }

  Vec &operator+=(const Vec<T, 2, C> &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vec &operator-=(const Vec<T, 2, C> &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vec &operator*=(T factor) {
    x *= factor;
    y *= factor;
    return *this;
  }

  Vec &operator/=(T factor) {
    x /= factor;
    y /= factor;
    return *this;
  }

  Vec operator+(const Vec<T, 2, C> &rhs) const {
    return Vec<T, 2, C>(x + rhs.x, y + rhs.y);
  }

  Vec operator-(const Vec<T, 2, C> &rhs) const {
    return Vec<T, 2, C>(x - rhs.x, y - rhs.y);
  }

  Vec operator*(T factor) const { return Vec<T, 2, C>(x * factor, y * factor); }

  Vec operator/(T factor) const { return Vec<T, 2, C>(x / factor, y / factor); }

  // Prefer accessing x and y directly to this
  T &operator[](size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return *(T *)nullptr; // Not quite safe
    }
  }

  // Prefer accessing x and y directly to this
  T operator[](size_t index) const {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return *(volatile T *)nullptr; // Not quite safe
    }
  }

  bool operator==(const Vec<T, 2, C> &rhs) const {
    return x == rhs.x && y == rhs.y;
  }

  bool operator!=(const Vec<T, 2, C> &rhs) const { return !(*this == rhs); }

  T dot(const Vec<T, 2, C> &other) const { return x * other.x + y * other.y; }

  size_t size() const { return 2; }
};

typedef Vec<float, 2> Vec2f;
typedef Vec<double, 2> Vec2d;
typedef Vec<int8_t, 2> Vec2i8;
typedef Vec<int32_t, 2> Vec2i32;
typedef Vec<int64_t, 2> Vec2i64;
typedef Vec<uint8_t, 2> Vec2u8;
typedef Vec<uint32_t, 2> Vec2u32;
typedef Vec<uint64_t, 2> Vec2u64;

template <typename T, bool C> class Vec<T, 3, C> {
public:
  T x;
  T y;
  T z;

  Vec() {}

  template <typename U>
  Vec(const Vec<U, 3, C> &other)
      : x(other.x), y(other.y), z(other.z) {}

  template <typename U> Vec(std::initializer_list<U> list) {
    size_t i = 0;
    for (auto it = list.begin(); it != list.end() && i < 3; ++it, ++i) {
      (*this)[i] = *it;
    }
  }

  Vec(const Matrix<T, 1, 3> &other)
      : x(other[0][0]), y(other[0][1]), z(other[0][2]) {
    static_assert(C == false, "Mismatched vector/matrix dimensions");
  }

  Vec(const Matrix<T, 3, 1> &other)
      : x(other[0][0]), y(other[1][0]), z(other[2][0]) {
    static_assert(C == true, "Mismatched vector/matrix dimensions");
  }

  /*
   * Pitch should be within [-PI/2, PI/2], and yaw within [0, 2 * PI}
   */
  Vec(T _pitch, T _yaw)
      : x(cos(_pitch) * sin(_yaw)), y(sin(_pitch)), z(cos(_pitch) * cos(_yaw)) {
  }

  Vec(T x, T y, T z) : x(x), y(y), z(z) {}

  static Vec zeros() {
    Vec<T, 3, C> rval;
    rval.zero();
    return rval;
  }

  static Vec x_axis() { return Vec<T, 3, C>(T(1), T(0), T(0)); }

  static Vec y_axis() { return Vec<T, 3, C>(T(0), T(1), T(0)); }

  static Vec z_axis() { return Vec<T, 3, C>(T(0), T(0), T(1)); }

  ~Vec() {}

  decltype(sqrt(T())) magnitude() const {
    return sqrt(magnitude_square());
  }

  T magnitude_square() const { return x * x + y * y + z * z; }

  void zero() {
    x = T(0);
    y = T(0);
    z = T(0);
  }

  Vec normal() { return *this / magnitude(); }

  void normalize() { *this = normal(); }

  Vec &operator+=(const Vec<T, 3, C> &rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }

  Vec &operator-=(const Vec<T, 3, C> &rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }

  Vec &operator*=(T factor) {
    x *= factor;
    y *= factor;
    z *= factor;
    return *this;
  }

  Vec &operator/=(T factor) {
    x /= factor;
    y /= factor;
    z /= factor;
    return *this;
  }

  Vec operator+(const Vec<T, 3, C> &rhs) const {
    return Vec<T, 3, C>(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  Vec operator-(const Vec<T, 3, C> &rhs) const {
    return Vec<T, 3, C>(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  Vec operator*(T factor) const {
    return Vec<T, 3, C>(x * factor, y * factor, z * factor);
  }

  Vec operator/(T factor) const {
    return Vec<T, 3, C>(x / factor, y / factor, z / factor);
  }

  // Prefer accessing x, y, and z directly to this
  T &operator[](size_t index) {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      return *(T *)nullptr; // Not quite safe
    }
  }

  // Prefer accessing x, y, and z directly to this
  T operator[](size_t index) const {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      return *(volatile T *)nullptr; // Not quite safe
    }
  }

  bool operator==(const Vec<T, 3, C> &rhs) const {
    return x == rhs.x && y == rhs.y && z == rhs.z;
  }

  bool operator!=(const Vec<T, 3, C> &rhs) const { return !(*this == rhs); }

  T dot(const Vec<T, 3, C> &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  size_t size() const { return 3; }

  /* ^
   * |
   * |
   * |
   * Y  (changes in height along Y)
   * |
   * |
   * |
   * v
   *  <--------X-------->
   */

  // A vector laying entirely within the xz plane has a pitch of 0.0
  // As the vector points towards the positive y axis, it's pitch increases
  // As the vector points towards the negative y axis, it's pitch decreases
  decltype(atan2(sqrt(T() * T() + T() * T()), T())) pitch() const {
    return atan2(y, sqrt(x * x + z * z));
  }

  // A vector pointing entirely along the positive z axis has a yaw of 0.0
  // As the vector points towards the positive x axis, it's yaw increases
  // As the vector points towards the negative x axis, it's yaw decreases
  decltype(atan2(T(), T())) yaw() const { return atan2(x, z); }

  // Rotates this vector around axis according to the right hand rule by theta
  // radians
  Vec rotation(const Vec<T, 3> &axis, T theta) {
#if 1
    return *this * cos(theta) + (axis.cross(*this)) * sin(theta) +
            axis * (axis.dot(*this)) * (1 - cos(theta));
#else

#endif
  }

  void rotate(const Vec<T, 3> &axis, T theta) {
    *this = rotation(axis, theta);
  }

  void rotate_x(T theta) {
#if 0
  rotate(x_axis(), theta);
#else
  auto c = cos(theta);
  auto s = sin(theta);
  auto temp = y * c - z * s;
  z = y * s + z * c;
  y = temp;
#endif
  }

  void rotate_y(T theta) {
#if 0
  rotate(y_axis(), theta);
#else
  auto c = cos(theta);
  auto s = sin(theta);
  auto temp = x * c + z * s;
  z = -x * s + z * c;
  x = temp;
#endif
  }

  void rotate_z(T theta) {
#if 0
  rotate(z_axis(), theta);
#else
  auto c = cos(theta);
  auto s = sin(theta);
  auto temp = x * c - y * s;
  y = x * s + y * c;
  x = temp;
#endif
  }

  // Reflects this vector along a plane specified by normal
  Vec reflection(const Vec<T, 3, C> &normal) {
    T d = this->dot(normal);
    return Vec<T, 3, C>(x - 2 * d * normal.x, y - 2 * d * normal.y,
                        z - 2 * d * normal.z);
  }

  Vec reflect(const Vec<T, 3, C> &normal) {
    *this = reflection(normal);
  }

  Vec cross(const Vec<T, 3, C> &other) const {
    return Vec<T, 3, C>(y * other.z - z * other.y, z * other.x - x * other.z,
                        x * other.y - y * other.x);
  }
};

typedef Vec<float, 3> Vec3f;
typedef Vec<double, 3> Vec3d;
typedef Vec<int8_t, 3> Vec3i8;
typedef Vec<int32_t, 3> Vec3i32;
typedef Vec<int64_t, 3> Vec3i64;
typedef Vec<uint8_t, 3> Vec3u8;
typedef Vec<uint32_t, 3> Vec3u32;
typedef Vec<uint64_t, 3> Vec3u64;
}

#endif
