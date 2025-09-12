#ifndef SCS_VECTOR_H
#define SCS_VECTOR_H

#include "encoding.h"

namespace scs {

// Specialization for std::vector<T>
template <typename T>
struct Encoding<std::vector<T>> {
    static void write_payload(std::vector<char>& buffer, const std::vector<T>& data) {
        SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
        Encoding<SER_DATA_LEN_T>::write_payload(buffer, length);
        for (const auto& item : data) {
            Encoding<T>::write_payload(buffer, item);
        }
    }

    static std::vector<T> read_payload(const char*& data) {
        SER_DATA_LEN_T length = Encoding<SER_DATA_LEN_T>::read_payload(data);
        std::vector<T> vec;
        vec.reserve(length);
        for (SER_DATA_LEN_T i = 0; i < length; ++i) {
            vec.push_back(Encoding<T>::read_payload(data));
        }
        return vec;
    }
};

}   // namespace scs

#endif