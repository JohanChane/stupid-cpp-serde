#ifndef SCS_SERIALIZER_H
#define SCS_SERIALIZER_H

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <sstream>
#include <iomanip>
#include <cstdint>

#include "members.h"
#include "string.h"
#include "vector.h"
#include "map.h"

namespace scs {

template <typename T>
struct SerializeHelper {
    static void serialize(std::vector<char>& buffer, const T& value) {
        Encoding<T>::write_payload(buffer, value);
    }

    static T deserialize(const char*& data) {
        return Encoding<T>::read_payload(data);
    }
};

// C++17 deduction guide
template <typename T> SerializeHelper(T) -> SerializeHelper<T>;

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
            Encoding<SER_DATA_LEN_T>::write_payload(buffer, length);
            buffer.insert(buffer.end(), nested_buffer.begin(), nested_buffer.end());
        } else {
            Encoding<typename MemberDesc::MemberType>::write_payload(buffer, value);
        }
    }

    template <typename MemberDesc>
    static void deserialize_member(T& obj, const char*& data) {
        auto& value = obj.*(MemberDesc::value);

        if constexpr (has_members_v<typename MemberDesc::MemberType>) {
            SER_DATA_LEN_T length = Encoding<SER_DATA_LEN_T>::read_payload(data);
            std::vector<char> nested_buffer(data, data + length);
            data += length;
            value = Serializer<typename MemberDesc::MemberType>::deserialize(nested_buffer);
        } else {
            value = Encoding<typename MemberDesc::MemberType>::read_payload(data);
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
        else if constexpr (std::is_same_v<typename MemberDesc::MemberType, std::map<int32_t, std::string>>) {
            std::cout << "{";
            bool first = true;
            for (const auto& [k, v] : value) {
                if (!first) std::cout << ", ";
                std::cout << k << ": \"" << v << "\"";
                first = false;
            }
            std::cout << "}";
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
