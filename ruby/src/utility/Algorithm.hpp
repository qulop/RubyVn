#pragma once

#include "TypeTraits.hpp"

#include <utility/Definitions.hpp>
#include <utility/Assert.hpp>



namespace Ruby {
    /* Prefix function in vector form
        Time complexity: O(n)
        Space complexity: O(n) */
    inline Vector<size_t> getPrefixVector(std::string_view str) {
        Vector<size_t> pi(str.size(), 0);
        size_t j = 0;

        for (size_t i = 1; i < str.size(); i++) {
            while (j > 0 && str.at(i) != str.at(j))
                j = pi.at(j - 1);

            if (str.at(i) == str.at(j)) 
                ++j;

            pi[i] = j;
        }

        return pi;
    }

    /* Prefix function in scalar form
        Time complexity: O(n)
        Space complexity: O(n) */
    inline size_t getPrefixScalar(std::string_view str, size_t index) {
        RUBY_ASSERT_BASIC(index < str.size());

        return getPrefixVector(str).at(index);
    }

    /* Knuth-Morris-Pratt algorithm
        Time complexity: O()
        Space complexity: O(n) */
    inline i64 findSubString(std::string_view haystack, std::string_view needle) {
        i64 index = RUBY_BAD_INDEX;
        auto&& prefixes = std::move(getPrefixVector(haystack));


        return index;
    }

    /* Time complexity: O((to - from) / step)
       Space complexity: O((to - from) / step) */
    inline String createStringSlice(std::string_view src, size_t from, size_t to, size_t step=1) {
        RUBY_ASSERT_BASIC(to <= src.size());
        if (step == 0)
            return String{ "" };

        String res;
        res.reserve((to - from) / step);

        for (size_t i = from; i <= to; i += step) 
            res += src.at(i);
        return res;
    }  
} 
