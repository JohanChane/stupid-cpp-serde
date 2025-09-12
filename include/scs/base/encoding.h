#ifndef SCS_ENCODING_H
#define SCS_ENCODING_H

#include <vector>
#include <cstring>

namespace scs {

#ifndef SER_DATA_LEN_T
#define SER_DATA_LEN_T uint32_t
#endif

// Primary template for Encoding
template <typename T, typename = void>
struct Encoding {
    static void write_payload(std::vector<char>& buffer, const T& data) {
        static_assert(std::is_arithmetic_v<T>, "Only arithmetic types are supported");
        const char* ptr = reinterpret_cast<const char*>(&data);
        buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
    }

    static T read_payload(const char*& data) {
        static_assert(std::is_arithmetic_v<T>, "Only arithmetic types are supported");
        T value;
        std::memcpy(&value, data, sizeof(T));
        data += sizeof(T);
        return value;
    }
};

} // namespace scs

#endif