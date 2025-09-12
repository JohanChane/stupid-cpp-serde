#ifndef SCS_STRUCTURE_H
#define SCS_STRUCTURE_H

#include "macros.h"
#include "members.h"

// ==============================================
// SCS_STRUCTURE Macro Definitions
// ==============================================

#define SCS_MAP_ARGS(m, args, ...) \
    _SCS_EXPAND(_SCS_MAP_FIRST_ARGS(m, args, __VA_ARGS__))

#define _SCS_MAP_FIRST_ARGS(m, args, ...) \
    _SCS_IF_ELSE(_SCS_HAS_ARGS(__VA_ARGS__)) ( \
        m(_SCS_REMOVE_PARENS(args), _SCS_REMOVE_PARENS(_SCS_FIRST_ARG(__VA_ARGS__))) \
        _SCS_MAP_NEXT_ARGS(m, args, _SCS_REST_ARG(__VA_ARGS__)) \
    )(/* done */)

#define _SCS_MAP_NEXT_ARGS(m, args, ...) \
    _SCS_IF_ELSE(_SCS_HAS_ARGS(__VA_ARGS__)) ( \
        , m(_SCS_REMOVE_PARENS(args), _SCS_REMOVE_PARENS(_SCS_FIRST_ARG(__VA_ARGS__))) \
        _SCS_DEFER3(__SCS_MAP_NEXT_ARGS)()(m, args, _SCS_REST_ARG(__VA_ARGS__)) \
    )(/* done */)

#define __SCS_MAP_NEXT_ARGS() _SCS_MAP_NEXT_ARGS

#define _SCS_MEMBER_FIRST(type, member) \
    ::scs::Member<type, &type::member>

#define _SCS_MEMBER_LIST(type, ...) \
  SCS_MAP_ARGS(_SCS_MEMBER_FIRST, (type), __VA_ARGS__)

#define SCS_STRUCTURE(type, ...) \
using Members = ::scs::MemberList< \
    _SCS_MEMBER_LIST(type, __VA_ARGS__) \
>

#endif // SCS_STRUCTURE_H
