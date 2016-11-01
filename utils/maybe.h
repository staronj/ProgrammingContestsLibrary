#pragma once
// Jakub Staroń, 2016

#include "headers.h"
#include "operators.h"

namespace lib {

struct Nothing_t { };

/**
 * Haskell-like Nothing.
 */
constexpr Nothing_t Nothing;

/**
 * Haskell-like Maybe type.
 */
template <typename T>
struct Maybe {
public:
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using reference = value_type&;
  using const_reference = const value_type&;
  using self_type = Maybe;

  /**
   * Constructs empty maybe.
   */
  Maybe() = default;

  /**
   * Constructs empty maybe.
   */
  Maybe(Nothing_t): Maybe() { }

  /**
   * Constructs maybe initialized with given value.
   */
  Maybe(value_type value) {
    construct(std::move(value));
  }

  /**
   * Constructs maybe from other maybe.
   */
  Maybe(const Maybe& other) {
    *this = other;
  }

  /**
   * Constructs maybe from other maybe.
   *
   * Other maybe will be empty after this.
   */
  Maybe(Maybe&& other) {
    *this = std::move(other);
  }

  /**
   * Stores value in maybe.
   *
   * Destroys previous value (if any).
   */
  Maybe& operator=(value_type value) {
    destroy();
    construct(std::move(value));
  }

  /**
   * Assign maybe to other maybe.
   */
  Maybe& operator=(const Maybe& other) {
    destroy();
    if (other)
      construct(other.get());
  }

  /**
   * Moves maybe into other maybe.
   *
   * Moved maybe will be empty after that.
   */
  Maybe& operator=(Maybe&& other) {
    if (other) {
      construct(std::move(other.get()));
      other.destroy();
    }
  }

  /**
   * Returns true if maybe is not initialized.
   */
  bool empty() const {
    return !initialized_;
  }

  /**
   * Returns true if maybe stores a value.
   */
  operator bool() const {
    return !empty();
  }

  /**
   * Returns stored value.
   *
   * If maybe is empty, throws.
   */
  reference get() {
    if (empty())
        throw std::logic_error("Empty maybe!");
    
    return storage_.ref();
  }  

  /**
   * Returns stored value.
   *
   * If maybe is empty throws.
   */
  const_reference get() const {
    if (empty())
        throw std::logic_error("Empty maybe!");
    
    return storage_.ref();
  }  

  friend bool operator==(const self_type& lhs, Nothing_t) {
      return lhs.empty();
  }

  friend bool operator==(Nothing_t, const self_type& rhs) {
      return rhs.empty();
  }
  
  ~Maybe() {
    destroy();
  }

private:
  void construct(value_type value) {
    storage_.construct(std::move(value));
    initialized_ = true;
  }

  void destroy() {
    if (initialized_)
      storage_.destroy();
    initialized_ = false;
  }

  struct storage_type {
    void construct(value_type value) {
        new (data_) value_type(value);
    }

    void destroy() {
        ~ref();   
    }

    pointer ptr() {
        return reinterpret_cast<pointer>(data_);
    }

    const_pointer ptr() const {
        return reinterpret_cast<const_pointer>(data_);
    }

    reference ref() {
        return *ptr();
    }

    const_reference ref() const {
        return *ptr();
    }
 
  private:
    byte data_[sizeof(value_type)];
  };

  storage_type storage_;
  bool initialized_ = false;
};

/**
 * Haskell-like Just(..) constructor.
 *
 * Example:
 * <pre>
 * auto maybeResult = [](int n) {
 *   return (n < 0)? Nothing : Just(n);
 * };
 * maybeResult(-1); // returns empty maybe
 * maybeResult(2); // returns maybe with value 2
 * </pre>
 */
template <typename T>
Maybe<T> Just(T value) {
  return Maybe<T>(std::move(value));
}


} // namespace lib
