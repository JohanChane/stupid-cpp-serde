#ifndef SCS_SERIALIZER_H
#define SCS_SERIALIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "members.h"
#include "structure.h"

namespace scs {

#ifndef SER_DATA_LEN_T
#define SER_DATA_LEN_T uint32_t
#endif

// Serialization functions
template <typename T>
void serialize_data(std::vector<char>& buffer, const T& data) {
    static_assert(std::is_arithmetic_v<T>, "Only arithmetic types are supported");
    const char* ptr = reinterpret_cast<const char*>(&data);
    buffer.insert(buffer.end(), ptr, ptr + sizeof(T));
}

template <>
inline void serialize_data<std::string>(std::vector<char>& buffer, const std::string& data) {
    SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
    serialize_data(buffer, length);
    buffer.insert(buffer.end(), data.begin(), data.end());
}

template <>
inline void serialize_data<std::vector<int32_t>>(std::vector<char>& buffer, const std::vector<int32_t>& data) {
    SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
    serialize_data(buffer, length);
    for (const auto& item : data) {
        serialize_data(buffer, item);
    }
}

template <>
inline void serialize_data<std::vector<std::string>>(std::vector<char>& buffer, const std::vector<std::string>& data) {
    SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(data.size());
    serialize_data(buffer, length);
    for (const auto& item : data) {
        serialize_data(buffer, item);
    }
}

// Deserialization functions
template <typename T>
T deserialize_data(const char*& data) {
    static_assert(std::is_arithmetic_v<T>, "Only arithmetic types are supported");
    T value;
    std::memcpy(&value, data, sizeof(T));
    data += sizeof(T);
    return value;
}

template <>
inline std::string deserialize_data<std::string>(const char*& data) {
    SER_DATA_LEN_T length = deserialize_data<SER_DATA_LEN_T>(data);
    std::string str(data, data + length);
    data += length;
    return str;
}

template <>
inline std::vector<int32_t> deserialize_data<std::vector<int32_t>>(const char*& data) {
    SER_DATA_LEN_T length = deserialize_data<SER_DATA_LEN_T>(data);
    std::vector<int32_t> vec;
    vec.reserve(length);
    for (SER_DATA_LEN_T i = 0; i < length; ++i) {
        vec.push_back(deserialize_data<int32_t>(data));
    }
    return vec;
}

template <>
inline std::vector<std::string> deserialize_data<std::vector<std::string>>(const char*& data) {
    SER_DATA_LEN_T length = deserialize_data<SER_DATA_LEN_T>(data);
    std::vector<std::string> vec;
    vec.reserve(length);
    for (SER_DATA_LEN_T i = 0; i < length; ++i) {
        vec.push_back(deserialize_data<std::string>(data));
    }
    return vec;
}

// Serializer class
template <typename T>
struct Serializer {
    static std::vector<char> serialize(const T& obj) {
        std::vector<char> buffer;
        serialize_members(obj, typename T::Members{}, buffer);
        return buffer;
    }

    static T deserialize(const std::vector<char>& buffer) {
        T obj;
        const char* data = buffer.data();
        deserialize_members(obj, typename T::Members{}, data);
        return obj;
    }

    static void debug_print(const T& obj, int indent = 0) {
        std::string indent_str(indent * 2, ' ');
        std::cout << indent_str << "Start printing " << typeid(T).name() << " object\n";
        debug_print_members(obj, typename T::Members{}, indent);
        std::cout << indent_str << "Print complete\n\n";
    }

private:
    template <typename Head, typename... Tail>
    static void serialize_members(const T& obj, MemberList<Head, Tail...>, std::vector<char>& buffer) {
        serialize_member<Head>(obj, buffer);
        serialize_members(obj, MemberList<Tail...>{}, buffer);
    }

    static void serialize_members(const T&, MemberList<>, std::vector<char>&) {}

    template <typename Head, typename... Tail>
    static void deserialize_members(T& obj, MemberList<Head, Tail...>, const char*& data) {
        deserialize_member<Head>(obj, data);
        deserialize_members(obj, MemberList<Tail...>{}, data);
    }

    static void deserialize_members(T&, MemberList<>, const char*&) {}

    template <typename Head, typename... Tail>
    static void debug_print_members(const T& obj, MemberList<Head, Tail...>, int indent) {
        debug_print_member<Head>(obj, indent);
        debug_print_members(obj, MemberList<Tail...>{}, indent);
    }

    static void debug_print_members(const T&, MemberList<>, int) {}

    template <typename MemberDesc>
    static void serialize_member(const T& obj, std::vector<char>& buffer) {
        const auto& value = obj.*(MemberDesc::value);
        
        if constexpr (has_members_v<typename MemberDesc::MemberType>) {
            auto nested_buffer = Serializer<typename MemberDesc::MemberType>::serialize(value);
            SER_DATA_LEN_T length = static_cast<SER_DATA_LEN_T>(nested_buffer.size());
            serialize_data(buffer, length);
            buffer.insert(buffer.end(), nested_buffer.begin(), nested_buffer.end());
        } else {
            serialize_data(buffer, value);
        }
    }

    template <typename MemberDesc>
    static void deserialize_member(T& obj, const char*& data) {
        auto& value = obj.*(MemberDesc::value);
        
        if constexpr (has_members_v<typename MemberDesc::MemberType>) {
            SER_DATA_LEN_T length = deserialize_data<SER_DATA_LEN_T>(data);
            std::vector<char> nested_buffer(data, data + length);
            data += length;
            value = Serializer<typename MemberDesc::MemberType>::deserialize(nested_buffer);
        } else {
            value = deserialize_data<typename MemberDesc::MemberType>(data);
        }
    }

    template <typename MemberDesc>
    static void debug_print_member(const T& obj, int indent) {
        const auto& value = obj.*(MemberDesc::value);
        std::string indent_str(indent * 2, ' ');
        
        std::cout << indent_str << "Field type: " << MemberDesc::type_name() 
                  << ", value: ";
        
        if constexpr (has_members_v<typename MemberDesc::MemberType>) {
            std::cout << "(nested struct)" << std::endl;
            Serializer<typename MemberDesc::MemberType>::debug_print(value, indent + 1);
            return;
        }
        else if constexpr (std::is_same_v<typename MemberDesc::MemberType, std::vector<int32_t>>) {
            std::cout << "[";
            for (size_t i = 0; i < value.size(); ++i) {
                std::cout << value[i];
                if (i < value.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        }
        else if constexpr (std::is_same_v<typename MemberDesc::MemberType, std::vector<std::string>>) {
            std::cout << "[";
            for (size_t i = 0; i < value.size(); ++i) {
                std::cout << "\"" << value[i] << "\"";
                if (i < value.size() - 1) std::cout << ", ";
            }
            std::cout << "]";
        }
        else if constexpr (std::is_same_v<typename MemberDesc::MemberType, std::string>) {
            std::cout << "\"" << value << "\"";
        }
        else {
            std::cout << value;
        }
        
        std::cout << std::endl;
    }
};

// Helper function to print buffer
inline void print_buffer(const std::vector<char>& buffer) {
    std::cout << "Serialization result (" << buffer.size() << " bytes):\n";
    std::ostringstream oss;
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (i > 0 && i % 16 == 0) oss << "\n";
        oss << std::hex << std::setw(2) << std::setfill('0') 
            << (static_cast<int>(buffer[i]) & 0xff) << " ";
    }
    std::cout << oss.str() << "\n\n";
}

} // namespace scs

#endif // SCS_SERIALIZER_H