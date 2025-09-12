#ifndef SCS_TYPE_NAME_H
#define SCS_TYPE_NAME_H

#include <typeinfo>
#include <vector>
#include <string>
#include <map>

template <typename T>
struct TypeName {
    static const char* get() {
        return typeid(T).name(); // or return "unknown"
    }
};

template <> struct TypeName<int32_t> { static const char* get() { return "int32_t"; } };
template <> struct TypeName<double> { static const char* get() { return "double"; } };
template <> struct TypeName<std::string> { static const char* get() { return "string"; } };
template <typename T> struct TypeName<std::vector<T>> {
    static const char* get() {
        static std::string name = std::string("vector<") + TypeName<T>::get() + ">";
        return name.c_str();
    }
};
template <typename K, typename V> struct TypeName<std::map<K, V>> {
    static const char* get() {
        static std::string name = std::string("map<") + TypeName<K>::get() + "," + TypeName<V>::get() + ">";
        return name.c_str();
    }
};

#endif