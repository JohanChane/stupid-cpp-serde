#ifndef SCS_STRING_H
#define SCS_STRING_H

#include  <string>
#include "encoding.h"

namespace scs {

// Specialization for std::string
template <>
struct Encoding<std::string> {
    static void write_payload(std::vector<char>& buffer, const std::string& data) {
        SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
        Encoding<SER_DATA_LEN_T>::write_payload(buffer, length);
        buffer.insert(buffer.end(), data.begin(), data.end());
    }

    static std::string read_payload(const char*& data) {
        SER_DATA_LEN_T length = Encoding<SER_DATA_LEN_T>::read_payload(data);
        std::string str(data, data + length);
        data += length;
        return str;
    }
};

}   // namespace scs

#endif