#pragma once
// Jakub Staroń, 2016

#include "headers.h"

namespace lib {

// post-incrementation -> pre-incrementation
template <typename T>
inline T operator++(T& obj, int) {
  T temp = obj;
  ++obj;
  return temp;
}

// post-decrementation -> pre-decrementation
template <typename T>
inline T operator--(T& obj, int) {
  T temp = obj;
  --obj;
  return temp;
}

// obj += n -> obj + n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T&>::type
operator+=(T& obj, Integral n) {
  obj = obj + n;
  return obj;
}

// obj1 += obj2 -> obj1 + obj2
template <typename T>
inline T& operator+=(T& obj1, const T& obj2) {
  obj1 = obj1 + obj2;
  return obj1;
}

// obj -= n -> obj - n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T&>::type
operator-=(T& obj, Integral n) {
  obj = obj - n;
  return obj;
}

// obj1 -= obj2 -> obj1 - obj2
template <typename T>
inline T& operator-=(T& obj1, const T& obj2) {
  obj1 = obj1 - obj2;
  return obj1;
}

// obj *= n -> obj * n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T&>::type
operator*=(T& obj, Integral n) {
  obj = obj * n;
  return obj;
}

// obj1 *= obj2 -> obj1 * obj2
template <typename T>
inline T& operator*=(T& obj1, const T& obj2) {
  obj1 = obj1 * obj2;
  return obj1;
}

// obj %= n -> obj % n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T&>::type
operator%=(T& obj, Integral n) {
  obj = obj % n;
  return obj;
}

// obj1 %= obj2 -> obj1 % obj2
template <typename T>
inline T& operator%=(T& obj1, const T& obj2) {
  obj1 = obj1 % obj2;
  return obj1;
}

// n + obj -> obj + n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T>::type
operator+(Integral n, const T& obj) {
  return obj + n;
}

// n * obj -> obj * n
template <typename T, typename Integral>
inline typename std::enable_if<std::is_integral<Integral>::value, T>::type
operator*(Integral n, const T& obj) {
  return obj * n;
}

// obj - n -> obj + (-n)
template <typename T, typename Integral>
inline typename std::enable_if<std::is_signed<Integral>::value, T>::type
operator-(const T& obj, Integral n) {
  return obj + (-n);
}

// obj -= n -> obj += (-n)
template <typename T, typename Integral>
inline typename std::enable_if<std::is_signed<Integral>::value, T&>::type
operator-=(const T& obj, Integral n) {
  return obj += (-n);
}

// lhs > rhs -> rhs < lhs
template <typename T>
inline bool operator>(const T& lhs, const T& rhs) {
  return rhs < lhs;
}

// lhs <= rhs -> !(rhs < lhs)
template <typename T>
inline bool operator<=(const T& lhs, const T& rhs) {
  return !(rhs < lhs);
}

// lhs >= rhs -> !(rhs < lhs)
template <typename T>
inline bool operator>=(const T& lhs, const T& rhs) {
  return rhs <= lhs;
}

// lhs >= rhs -> !(rhs < lhs)
template <typename T>
inline bool operator!=(const T& lhs, const T& rhs) {
  return !(lhs == rhs);
}

// + for pairs
template <typename Integral1, typename Integral2>
constexpr std::pair<Integral1, Integral2> operator+(const std::pair<Integral1, Integral2>& lhs, const std::pair<Integral1, Integral2>& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first + rhs.first, lhs.second + rhs.second);
}

// - for pairs
template <typename Integral1, typename Integral2>
constexpr std::pair<Integral1, Integral2> operator-(const std::pair<Integral1, Integral2>& lhs, const std::pair<Integral1, Integral2>& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first - rhs.first, lhs.second - rhs.second);
}

// % for pairs
template <typename Integral1, typename Integral2>
constexpr std::pair<Integral1, Integral2> operator%(const std::pair<Integral1, Integral2>& lhs, const std::pair<Integral1, Integral2>& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first % rhs.first, lhs.second % rhs.second);
}

// * for pairs
template <typename Integral1, typename Integral2>
constexpr std::pair<Integral1, Integral2> operator*(const std::pair<Integral1, Integral2>& lhs, const std::pair<Integral1, Integral2>& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first * rhs.first, lhs.second * rhs.second);
}

// % for (pair, Integral)
template <typename Integral1, typename Integral2, typename Integral3>
constexpr std::pair<Integral1, Integral2> operator%(const std::pair<Integral1, Integral2>& lhs, const Integral3& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first % rhs, lhs.second % rhs);
}

// * for (pair, Integral)
template <typename Integral1, typename Integral2, typename Integral3>
constexpr std::pair<Integral1, Integral2> operator*(const std::pair<Integral1, Integral2>& lhs, const Integral3& rhs) {
  return std::pair<Integral1, Integral2>(lhs.first * rhs, lhs.second * rhs);
}


} // namespace lib
