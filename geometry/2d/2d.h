#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "numeric.h"
#include "io.h"

namespace lib {
namespace geometry {
namespace _2d {

namespace detail {

template <typename T>
struct ProductTypeDeductor;

template <>
struct ProductTypeDeductor<int8> {
  using type = int16;
};

template <>
struct ProductTypeDeductor<int16> {
  using type = int32;
};

template <>
struct ProductTypeDeductor<int32> {
  using type = int64;
};

template <typename T>
struct ProductTypeDeductor {
  using type = T;
};

template <typename T, typename Type>
using enable_if_floating = typename std::enable_if<std::is_floating_point<T>::value, Type>::type;

template <typename T, typename Type>
using enable_if_integral = typename std::enable_if<std::is_integral<T>::value, Type>::type;

} // namespace detail

template<typename T>
struct point {
  static_assert(!std::is_unsigned<T>::value, "T should not be unsigned!");

  using value_type = T;
  value_type x;
  value_type y;
};

template<typename T>
point<T> operator+(const point<T>& lhs, const point<T>& rhs) {
  return {
      lhs.x + rhs.x,
      lhs.y + rhs.y
  };
}

template<typename T>
point<T> operator-(const point<T>& lhs, const point<T>& rhs) {
  return {
      lhs.x - rhs.x,
      lhs.y - rhs.y
  };
}

template<typename T>
point<T> operator*(const point<T>& lhs, const T& rhs) {
  return {
      lhs.x * rhs,
      lhs.y * rhs
  };
}

template<typename T>
detail::enable_if_floating<T, point<T>> operator/(point<T>& lhs, const T& rhs) {
  return {
      lhs.x / rhs,
      lhs.y / rhs
  };
}

template<typename T>
void operator+=(point<T>& lhs, const point<T>& rhs) {
  lhs = lhs + rhs;
}

template<typename T>
void operator-=(point<T>& lhs, const point<T>& rhs) {
  lhs = lhs + rhs;
}

template<typename T>
void operator*=(point<T>& lhs, const T& rhs) {
  lhs = lhs * rhs;
}

template<typename T>
detail::enable_if_floating<T, void> operator/=(point<T>& lhs, const T& rhs) {
  lhs = lhs / rhs;
}

template<typename T>
detail::enable_if_integral<T, bool> operator==(const point<T>& lhs, const point<T>& rhs) {
  return lhs.x == rhs.x && lhs.y == rhs.y;
}

template<typename T>
detail::enable_if_integral<T, bool> operator!=(const point<T>& lhs, const point<T>& rhs) {
  return !(lhs == rhs);
}

template<typename T>
typename detail::ProductTypeDeductor<T>::type ScalarProduct(const point<T>& lhs, const point<T>& rhs) {
  using product_type = typename detail::ProductTypeDeductor<T>::type;
  return
      product_type(lhs.x) * product_type(rhs.x) +
      product_type(lhs.y) * product_type(rhs.y);
}

template<typename T>
typename detail::ProductTypeDeductor<T>::type VectorProduct(const point<T>& lhs, const point<T>& rhs) {
  using product_type = typename detail::ProductTypeDeductor<T>::type;
  return
      product_type(lhs.x) * product_type(rhs.y) -
      product_type(lhs.y) * product_type(rhs.x);
}

template<typename T>
detail::enable_if_floating<T, T> length(const point<T>& lhs) {
  T product = ScalarProduct(lhs, lhs);
  return std::sqrt(product);
}

template<typename T>
detail::enable_if_floating<T, point<T>>
normalize(const point<T>& point) {
  return point / length(point);
}

template<typename T>
detail::enable_if_floating<T, T>
sin(const point<T>& lhs, const point<T>& rhs) {
  auto lhs_n = normalize(lhs);
  auto rhs_n = normalize(rhs);
  return VectorProduct(lhs, rhs);
}

template<typename T>
detail::enable_if_floating<T, T>
cos(const point<T>& lhs, const point<T>& rhs) {
  auto lhs_n = normalize(lhs);
  auto rhs_n = normalize(rhs);
  return ScalarProduct(lhs, rhs);
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const point<T>& point) {
  lib::detail::delimiter_printer printer(stream);
  stream << printer.prefix();
  stream << printer.delimiter() << point.x;
  stream << printer.delimiter() << point.y;
  stream << printer.postfix();
  return stream;
}

template<typename T>
std::istream& operator>>(std::istream& stream, point<T>& point) {
  return stream >> point.x >> point.y;
}

template<typename T>
struct Segment {
  point<T> first;
  point<T> second;
};

template<typename T>
struct Line {
  Line(const point<T>& first, const point<T>& second):
      Line::first(first),
      Line::second(second) { }

  Line(const Segment<T>& segment):
      first(segment.first),
      second(segment.second) { }

  point<T> first;
  point<T> second;
};

template<typename T>
detail::enable_if_floating<T, T> distanceToLine(const point<T>& point, const Line<T>& line) {
  auto line_vector = normalize(line.first - line.second);
  return lib::abs(VectorProduct(line_vector, point - line.first));
}

template<typename T>
struct Polygon {
  template <typename Iterator>
  Polygon(Iterator begin, Iterator end):
      points(begin, end) { }

  Polygon(std::initializer_list<point<T>> list):
      points(list) { }

  std::vector<point<T>> points;
};

} // namespace _2d
} // namespace geometry
} // namespace lib
