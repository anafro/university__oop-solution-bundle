/*
 * -----------------------------------------------
 * Strings
 * -----------------------------------------------
 * It contains useful function extending standard
 * std::string functionality.
 *
 * @since 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include <vector>
#include <string>
#include <cstring>

namespace PROJECT_NAME {
    static std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> strings;

        std::string::size_type pos = 0;
        std::string::size_type prev = 0;
        while ((pos = str.find(delimiter, prev)) != std::string::npos)
        {
            strings.push_back(str.substr(prev, pos - prev));
            prev = pos + 1;
        }

        strings.push_back(str.substr(prev));

        return strings;
    }

    static size_t longest_similar_subsequence(const char* s1, const char* s2)
    {
        size_t len[3];

        if (*s1 == '\0' || *s2 == '\0') return 0;

        len[0] = (*s1 == *s2) + longest_similar_subsequence(s1 + 1, s2 + 1);
        len[1] = longest_similar_subsequence(s1 + 1, s2);
        len[2] = longest_similar_subsequence(s1, s2 + 1);

        if (len[0] < len[1]) len[0] = len[1];
        if (len[0] < len[2]) len[0] = len[2];

        return len[0];
    }

    static size_t levenshtein_distance(const std::string& first, const std::string& second) {
        const size_t first_length(first.size()), second_length(second.size());

        if(first_length == 0) return second_length;
        if(second_length == 0) return first_length;

        size_t costs[second_length + 1];

        for(size_t cost = 0; cost <= second_length; cost++) {
            costs[cost] = cost;
        }

        size_t index_first = 0;
        for(const char& character_from_first : first) {
            costs[0] = index_first + 1;
            size_t corner = index_first, index_second = 0;
            for(const char& character_from_second : second) {
                size_t upper { costs[index_second + 1] };
                if(character_from_first == character_from_second) {
                    costs[index_second + 1] = corner;
                } else {
                    size_t t(upper < corner ? upper: corner);
                    costs[index_second + 1] = (costs[index_second] < t ? costs[index_second] : t) + 1;
                }

                corner = upper;
                index_second++;
            }
            index_first++;
        }

        return costs[second_length];
    }

    static double similarity(const std::string& s1, const std::string& s2) {
        size_t s1len = s1.length();
        size_t s2len = s2.length();
        double computed_similarity;

        if (s1len == 0 && s2len == 0)
        {
            // Two empty strings are equal
            computed_similarity = 1;
        }
        else
        {
            size_t len;
            // Append s1 to itself in s1s1 (e.g. "aircon" -> "airconaircon")
            char* doubled_s1 = (char*) malloc(s1len * 2 + 1);
            strcpy(doubled_s1, s1.c_str());
            strcpy(doubled_s1 + s1len, s1.c_str());

            // Find the length of the LCS between s1s1 and s2
            // (e.g. between "airconaircon" and "conair")
            len = longest_similar_subsequence(doubled_s1, s2.c_str());
            // We need it not longer than s1 (e.g. "aircon")
            // since we're actually comparing s1 and s2
            if(len > s1len)
                len = s1len;

            len *= 2;

            // Prevent 100% similarity between a string and its
            // cyclically shifted version (e.g. "aircon" and "conair")
            if (len == s1len + s2len && s1.compare(s2) != 0) len--;

            // Get the final measure of the similarity
            computed_similarity = (double)len / (double)(s1len + s2len);

            free(doubled_s1);
        }

        return computed_similarity;
    }
}