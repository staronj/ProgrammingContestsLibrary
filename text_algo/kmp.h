//  Michał Wiatrowski

#pragma once

#include "headers.h"

namespace lib {

class KMP {
public:
    template <typename iterator>
    void feed(iterator begin, iterator end);
    const std::vector<int>& get_array();

private:
    std::vector<int> prefix_suffix_array;
};

template <typename iterator>
void KMP::feed(iterator begin, iterator end) {
    std::vector<int>& array = prefix_suffix_array;

    int size = std::distance(begin, end);
    array.resize(size);
    
    if (size == 0)
        return;

    array[0] = 0;
    int longest_ps = 0;
    for (int i : range(1, size)) {
        while (longest_ps > 0) {
            if (*(begin + i) != *(begin + longest_ps)) {
                longest_ps = array[longest_ps - 1];
            } else {
                longest_ps += 1;
                break;
            }
        }

        if (longest_ps == 0) {
            if (*(begin) == *(begin + i))
                longest_ps = 1;
        }

        array[i] = longest_ps;
    }
}

const std::vector<int>& KMP::get_array() {
    return prefix_suffix_array;
}

}   //  namespace lib