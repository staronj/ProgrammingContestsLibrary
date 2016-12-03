// Jakub Staroń, 2016

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include "iterators.h"

using namespace lib;

BOOST_AUTO_TEST_SUITE(is_iterator_test_suite)

BOOST_AUTO_TEST_CASE(is_iterator_test) {
  BOOST_CHECK_EQUAL(is_iterator<const char*>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<char>::value, false);
  BOOST_CHECK_EQUAL(is_iterator<int*>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<int* const>::value, false);
  BOOST_CHECK_EQUAL(is_iterator<int>::value, false);
  BOOST_CHECK_EQUAL(is_iterator<std::vector<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::vector<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::forward_list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::forward_list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::set<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::set<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<const std::vector<int>::iterator>::value, false);
  BOOST_CHECK_EQUAL(is_iterator<std::istream_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::ostream_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<std::back_insert_iterator<std::vector<int>>>::value, true);

  BOOST_CHECK_EQUAL(is_iterator<counting_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_iterator<counting_iterator<uint32>>::value, true);

  int table[4];
  BOOST_CHECK_EQUAL(is_iterator<decltype(table)>::value, false);
  BOOST_CHECK_EQUAL(is_iterator<decltype(std::begin(table))>::value, true);
}

BOOST_AUTO_TEST_CASE(is_iterator_concept_test) {
  {
    struct ValidIterator {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = ValidIterator;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }
    };

    BOOST_CHECK_EQUAL(is_iterator<ValidIterator>::value, true);
  }

  {
    struct NonDeferencable {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = NonDeferencable;

      self_type& operator++() { return *this; }
    };

    BOOST_CHECK_EQUAL(is_iterator<NonDeferencable>::value, false);
  }

  {
    struct NonIncrementable {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = NonIncrementable;

      reference operator*() { return *(new int()); }
    };

    BOOST_CHECK_EQUAL(is_iterator<NonIncrementable>::value, false);
  }

  {
    struct WithoutCategory {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using self_type = WithoutCategory;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }
    };

    BOOST_CHECK_EQUAL(is_iterator<WithoutCategory>::value, false);
  }

  {
    struct NonDestructible {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = NonDestructible;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      ~NonDestructible() = delete;
    };

    BOOST_CHECK_EQUAL(is_iterator<NonDestructible>::value, false);
  }

  {
    struct WrongIncrementResultType {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = WrongIncrementResultType;

      int operator++() { return 0; }
      reference operator*() { return *(new int()); }
    };

    BOOST_CHECK_EQUAL(is_iterator<WrongIncrementResultType>::value, false);
  }
}

BOOST_AUTO_TEST_CASE(is_input_iterator_test) {
  BOOST_CHECK_EQUAL(is_input_iterator<const char*>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<int*>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::vector<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::vector<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::forward_list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::forward_list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::set<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::set<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::istream_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<std::ostream_iterator<int>>::value, false);
  BOOST_CHECK_EQUAL(is_input_iterator<std::back_insert_iterator<std::vector<int>>>::value, false);

  BOOST_CHECK_EQUAL(is_input_iterator<counting_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_input_iterator<counting_iterator<uint32>>::value, true);

  int table[4];
  BOOST_CHECK_EQUAL(is_input_iterator<decltype(std::begin(table))>::value, true);
}

BOOST_AUTO_TEST_CASE(is_input_iterator_concept_test) {
  {
    struct ValidIterator {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = ValidIterator;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_input_iterator<ValidIterator>::value, true);
  }

  {
    struct NotComparable {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = NotComparable;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }
    };

    BOOST_CHECK_EQUAL(is_input_iterator<NotComparable>::value, false);
  }

  {
    struct WrongDereference {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = WrongDereference;

      self_type& operator++() { return *this; }
      value_type operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_input_iterator<WrongDereference>::value, false);
  }

  {
    struct WrongTag {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::output_iterator_tag;
      using self_type = WrongTag;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_input_iterator<WrongTag>::value, false);
  }
}

BOOST_AUTO_TEST_CASE(is_forward_iterator_test) {
  BOOST_CHECK_EQUAL(is_forward_iterator<const char*>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<int*>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::vector<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::vector<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::forward_list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::forward_list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::set<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<std::set<int>::const_iterator>::value, true);

  BOOST_CHECK_EQUAL(is_forward_iterator<counting_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_forward_iterator<counting_iterator<uint32>>::value, true);

  int table[4];
  BOOST_CHECK_EQUAL(is_forward_iterator<decltype(std::begin(table))>::value, true);
}

BOOST_AUTO_TEST_CASE(is_forward_iterator_concept_test) {
  {
    struct ValidIterator {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::forward_iterator_tag;
      using self_type = ValidIterator;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_forward_iterator<ValidIterator>::value, true);
  }

  {
    struct InvalidCategory {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::input_iterator_tag;
      using self_type = InvalidCategory;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_forward_iterator<InvalidCategory>::value, false);
  }

  {
    struct WithoutIncrementation {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::forward_iterator_tag;
      using self_type = WithoutIncrementation;
      
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_forward_iterator<WithoutIncrementation>::value, false);
  }
}

BOOST_AUTO_TEST_CASE(is_bidirectional_iterator_test) {
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<const char*>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<int*>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::vector<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::vector<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::list<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::list<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::forward_list<int>::iterator>::value, false);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::forward_list<int>::const_iterator>::value, false);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::set<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<std::set<int>::const_iterator>::value, true);

  BOOST_CHECK_EQUAL(is_bidirectional_iterator<counting_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<counting_iterator<uint32>>::value, true);

  int table[4];
  BOOST_CHECK_EQUAL(is_bidirectional_iterator<decltype(std::begin(table))>::value, true);
}

BOOST_AUTO_TEST_CASE(is_bidirectional_iterator_concept_test) {
  {
    struct ValidIterator {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::bidirectional_iterator_tag;
      using self_type = ValidIterator;

      self_type& operator++() { return *this; }
      self_type& operator--() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_bidirectional_iterator<ValidIterator>::value, true);
  }

  {
    struct WrongTag {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::forward_iterator_tag;
      using self_type = WrongTag;

      self_type& operator++() { return *this; }
      self_type& operator--() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_bidirectional_iterator<WrongTag>::value, false);
  }

  {
    struct NoDecrementation {
      using value_type = int;
      using difference_type = int;
      using reference = int&;
      using pointer = int*;
      using iterator_category = std::bidirectional_iterator_tag;
      using self_type = NoDecrementation;

      self_type& operator++() { return *this; }
      reference operator*() { return *(new int()); }

      bool operator==(const self_type&) const { return true; }
    };

    BOOST_CHECK_EQUAL(is_bidirectional_iterator<NoDecrementation>::value, false);
  }
}

BOOST_AUTO_TEST_CASE(is_random_access_iterator_test) {
  BOOST_CHECK_EQUAL(is_random_access_iterator<const char*>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<int*>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::vector<int>::iterator>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::vector<int>::const_iterator>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::list<int>::iterator>::value, false);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::list<int>::const_iterator>::value, false);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::set<int>::iterator>::value, false);
  BOOST_CHECK_EQUAL(is_random_access_iterator<std::set<int>::const_iterator>::value, false);

  BOOST_CHECK_EQUAL(is_random_access_iterator<counting_iterator<int>>::value, true);
  BOOST_CHECK_EQUAL(is_random_access_iterator<counting_iterator<uint32>>::value, true);

  int table[4];
  BOOST_CHECK_EQUAL(is_random_access_iterator<decltype(std::begin(table))>::value, true);
}

BOOST_AUTO_TEST_SUITE_END()
