/*
 * -----------------------------------------------
 * Dictionary
 * -----------------------------------------------
 * Is a key-value container with unique keys.
 *
 * @since OOP 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include <vector>

namespace PROJECT_NAME {
    template<typename K, typename V>
    class Dictionary {
        using Key = K;
        using Value = V;

        std::vector<std::pair<Key, Value>> pairs;
    public:
        explicit Dictionary() = default;

        virtual ~Dictionary() = default;

        std::pair<Key, Value>& find_mutable_pair(const Key& key) {
            for(auto& pair : get_pairs()) {
                if(pair.first == key) {
                    return pair;
                }
            }

            throw std::runtime_error("Trying to get value from dictionary by key, which is not present.");
        }

        const auto& find_pair(const Key& key) const {
            for(const auto& pair : get_pairs()) {
                if(pair.first == key) {
                    return pair;
                }
            }

            throw std::runtime_error("Trying to get value from dictionary by key, which is not present.");
        }

        [[nodiscard]]
        bool has(const Key& key) const {
            try {
                find_pair(key);
                return true;
            } catch(const std::runtime_error&) {
                return false;
            }
        }

        void put(const Key& key, Value value) {
            if(has(key)) {
                pairs.erase(std::remove_if(pairs.begin(), pairs.end(), [&](const auto& pair) {
                    return pair.first == key;
                }), pairs.end());
            }

            pairs.push_back(std::pair<Key, Value>{key, value});
        }

        [[nodiscard]]
        std::vector<Key> get_keys() const {
            std::vector<Key> dictionary_keys;
            for(const auto& pair : get_pairs()) {
                dictionary_keys.push_back(pair.first);
            }

            return dictionary_keys;
        }

        [[nodiscard]]
        std::vector<Value> get_values() const {
            std::vector<Value> dictionary_keys;
            for(const auto& pair : get_pairs()) {
                dictionary_keys.push_back(pair.second);
            }

            return dictionary_keys;
        }

        [[nodiscard]]
        Value& get(const Key& key) {
            return find_mutable_pair(key).second;
        }

        [[nodiscard]]
        const Value& get(const Key& key) const {
            return find_pair(key).second;
        }

        [[nodiscard]]
        const Value& operator[](const Key& key) const {
            return get(key);
        }

        [[nodiscard]]
        Value& operator[](const Key& key) {
            return get(key);
        }

        [[maybe_unused]] [[nodiscard]]
        auto get_pairs() {
            return pairs;
        }

        [[nodiscard]]
        const auto& get_pairs() const {
            return pairs;
        }

        [[nodiscard]]
        size_t size() const {
            return pairs.size();
        }
    };
}