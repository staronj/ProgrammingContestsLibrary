#pragma once
// Jakub Staroń, 2016-2017

namespace pcl {

namespace detail {

template <bool B>
using enable_if = typename std::enable_if<B, bool>::type;

template <typename It>
using value_t = typename std::iterator_traits<It>::value_type;

template <typename It>
using difference_t = typename std::iterator_traits<It>::difference_type;

template <typename It>
using reference_t = typename std::iterator_traits<It>::reference;

template <typename It>
using pointer_t = typename std::iterator_traits<It>::pointer;

template <typename It>
using iterator_category_t = typename std::iterator_traits<It>::iterator_category;

} // namespace detail

/**
 * is_terator<T>::value is true if T satisfies iterator concept
 *
 * See http://en.cppreference.com/w/cpp/concept/Iterator for details.
 */
template<typename Iterator>
struct is_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      std::declval<detail::value_t<I>>(),
      std::declval<detail::difference_t<I>>(),
      std::declval<detail::reference_t<I>>(),
      std::declval<detail::pointer_t<I>>(),
      std::declval<detail::iterator_category_t<I>>(),
      *std::declval<I&>(),
      ++std::declval<I&>(),
      bool()) { return
      // note that we are not checking for being swappable
      // it's known flaw, but is_swappable is available only
      // since C++17
        std::is_copy_constructible<I>::value &&
        std::is_copy_assignable<I>::value &&
        std::is_destructible<I>::value &&
        std::is_same<decltype(++std::declval<I&>()), I&>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
  constexpr operator bool() const { return value; }
};

template<typename It>
constexpr bool is_iterator<It>::value;


/**
 * is_input_terator<T>::value is true if T satisfies input iterator concept
 *
 * See http://en.cppreference.com/w/cpp/concept/InputIterator for details.
 */
template<typename Iterator>
struct is_input_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      detail::enable_if<is_iterator<I>::value>(),
      std::declval<I&>()++,
      *std::declval<I&>()++,
      std::declval<const I&>() == std::declval<const I&>(),
      std::declval<const I&>() != std::declval<const I&>(),
      bool()) { return
        std::is_convertible<decltype(std::declval<const I&>() == std::declval<const I&>()), bool>::value &&
        std::is_convertible<decltype(std::declval<const I&>() != std::declval<const I&>()), bool>::value &&
        std::is_same<decltype(*std::declval<I&>()), detail::reference_t<I>>::value &&
        std::is_convertible<detail::reference_t<I>, detail::value_t<I>>::value &&
        std::is_convertible<decltype(*std::declval<I&>()++), detail::value_t<I>>::value &&
        std::is_base_of<std::input_iterator_tag, detail::iterator_category_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
  constexpr operator bool() const { return value; }
};

template<typename It>
constexpr bool is_input_iterator<It>::value;

/**
 * is_forward_terator<T>::value is true if T satisfies forward iterator concept
 *
 * See http://en.cppreference.com/w/cpp/concept/ForwardIterator for details.
 */
template<typename Iterator>
struct is_forward_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      detail::enable_if<is_input_iterator<I>::value>(),
      bool()) { return
        std::is_default_constructible<I>::value &&
        std::is_same<decltype(std::declval<I&>()++), I>::value &&
        std::is_same<decltype(*std::declval<I&>()++), detail::reference_t<I>>::value &&
        std::is_base_of<std::forward_iterator_tag, detail::iterator_category_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
  constexpr operator bool() const { return value; }
};

template<typename It>
constexpr bool is_forward_iterator<It>::value;

/**
 * is_bidirectional_terator<T>::value is true if T satisfies bidirectional iterator concept
 *
 * See http://en.cppreference.com/w/cpp/concept/BidirectionalIterator for details.
 */
template<typename Iterator>
struct is_bidirectional_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      detail::enable_if<is_forward_iterator<I>::value>(),
      --std::declval<I&>(),
      std::declval<I&>()--,
      *std::declval<I&>()--,
      bool()) { return
        std::is_same<decltype(--std::declval<I&>()), I&>::value &&
        std::is_convertible<decltype(std::declval<I&>()--), const I&>::value &&
        std::is_same<decltype(*std::declval<I&>()--), detail::reference_t<I>>::value &&
        std::is_base_of<std::bidirectional_iterator_tag, detail::iterator_category_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
  constexpr operator bool() const { return value; }
};

template<typename It>
constexpr bool is_bidirectional_iterator<It>::value;

/**
 * is_random_access_terator<T>::value is true if T satisfies random_access iterator concept
 *
 * See http://en.cppreference.com/w/cpp/concept/RandomAccessIterator for details.
 */
template<typename Iterator>
struct is_random_access_iterator {
private:
  template<typename I> static constexpr auto test(int)
  -> decltype(
      detail::enable_if<is_bidirectional_iterator<I>::value>(),
      std::declval<I&>() += std::declval<detail::difference_t<I>>(),
      std::declval<I&>() + std::declval<detail::difference_t<I>>(),
      std::declval<detail::difference_t<I>>() + std::declval<I&>(),
      std::declval<I&>() -= std::declval<detail::difference_t<I>>(),
      std::declval<I&>() - std::declval<detail::difference_t<I>>(),
      std::declval<I&>() - std::declval<I&>(),
      std::declval<I&>()[std::declval<detail::difference_t<I>>()],
      std::declval<I&>() < std::declval<I&>(),
      std::declval<I&>() <= std::declval<I&>(),
      std::declval<I&>() > std::declval<I&>(),
      std::declval<I&>() >= std::declval<I&>(),
      bool()) { return
        std::is_same<decltype(std::declval<I&>() += std::declval<detail::difference_t<I>>()), I&>::value &&
        std::is_same<decltype(std::declval<I&>() + std::declval<detail::difference_t<I>>()), I>::value &&
        std::is_same<decltype(std::declval<detail::difference_t<I>>() + std::declval<I&>()), I>::value &&
        std::is_same<decltype(std::declval<I&>() -= std::declval<detail::difference_t<I>>()), I&>::value &&
        std::is_same<decltype(std::declval<I&>() - std::declval<detail::difference_t<I>>()), I>::value &&
        std::is_same<decltype(std::declval<I&>() - std::declval<I&>()), detail::difference_t<I>>::value &&
        std::is_convertible<decltype(std::declval<I&>()[std::declval<detail::difference_t<I>>()]), detail::reference_t<I>>::value &&
        std::is_convertible<decltype(std::declval<I&>() < std::declval<I&>()), bool>::value &&
        std::is_convertible<decltype(std::declval<I&>() <= std::declval<I&>()), bool>::value &&
        std::is_convertible<decltype(std::declval<I&>() > std::declval<I&>()), bool>::value &&
        std::is_convertible<decltype(std::declval<I&>() >= std::declval<I&>()), bool>::value &&
        std::is_same<std::random_access_iterator_tag, detail::iterator_category_t<I>>::value;
  }

  template<typename I> static constexpr bool test(...) { return false; }

public:
  static constexpr bool value = test<Iterator>(0);
  constexpr operator bool() const { return value; }
};

template<typename It>
constexpr bool is_random_access_iterator<It>::value;

} // namespace pcl
