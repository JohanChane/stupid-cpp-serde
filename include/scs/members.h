#ifndef SCS_MEMBERS_H
#define SCS_MEMBERS_H

#include <type_traits>
#include <vector>
#include <string>
#include <tuple>

namespace scs {

// Type trait to check if a type has Members typedef
template <typename T, typename = void>
struct HasMembers : std::false_type {};

template <typename T>
struct HasMembers<T, std::void_t<typename T::Members>> : std::true_type {};

template <typename T>
inline constexpr bool has_members_v = HasMembers<T>::value;

// Member descriptor - stores information about struct members
template <typename T, auto Ptr>
struct Member {
    using ClassType = T;
    using MemberType = std::decay_t<decltype(std::declval<T>().*Ptr)>;
    static constexpr auto value = Ptr;
    
    static const char* type_name() {
        if constexpr (std::is_same_v<MemberType, int32_t>) return "int32_t";
        else if constexpr (std::is_same_v<MemberType, double>) return "double";
        else if constexpr (std::is_same_v<MemberType, std::string>) return "string";
        else if constexpr (std::is_same_v<MemberType, std::vector<int32_t>>) return "vector<int32_t>";
        else if constexpr (std::is_same_v<MemberType, std::vector<std::string>>) return "vector<string>";
        else if constexpr (has_members_v<MemberType>) return "struct";
        else return "unknown";
    }
};

// Member list - holds a list of Member descriptors
template <typename... Members>
struct MemberList {
    using Tuple = std::tuple<Members...>;
    static constexpr size_t size = sizeof...(Members);
};

} // namespace scs

#endif // SCS_MEMBERS_H
