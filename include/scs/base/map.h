#ifndef SCS_MAP_H
#define SCS_MAP_H

#include <map>
#include "encoding.h"

namespace scs {

// Specialization for std::map<K, V>
template <typename K, typename V>
struct Encoding<std::map<K, V>> {
    static void write_payload(std::vector<char>& buffer, const std::map<K, V>& data) {
        SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
        Encoding<SER_DATA_LEN_T>::write_payload(buffer, length);
        for (const auto& [key, value] : data) {
            Encoding<K>::write_payload(buffer, key);
            Encoding<V>::write_payload(buffer, value);
        }
    }

    static std::map<K, V> read_payload(const char*& data) {
        SER_DATA_LEN_T length = Encoding<SER_DATA_LEN_T>::read_payload(data);
        std::map<K, V> map;
        for (SER_DATA_LEN_T i = 0; i < length; ++i) {
            K key = Encoding<K>::read_payload(data);
            V value = Encoding<V>::read_payload(data);
            map.emplace(std::move(key), std::move(value));
        }
        return map;
    }
};

}   // namespace scs

#endif