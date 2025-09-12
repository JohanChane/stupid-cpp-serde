#ifndef SCS_MEMBERS_H
#define SCS_MEMBERS_H

#include <tuple>
#include "type_name.h"

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
        return TypeName<MemberType>::get();
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
