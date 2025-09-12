#ifndef SCS_MACROS_H
#define SCS_MACROS_H

// ==============================================
// SCS Macro Toolchain Definitions
// ==============================================

// Recursive expansion macros
#define _SCS_EXPAND0(...) __VA_ARGS__
#define _SCS_EXPAND1(...) _SCS_EXPAND0(_SCS_EXPAND0(_SCS_EXPAND0(__VA_ARGS__)))
#define _SCS_EXPAND2(...) _SCS_EXPAND1(_SCS_EXPAND1(_SCS_EXPAND1(__VA_ARGS__)))
#define _SCS_EXPAND3(...) _SCS_EXPAND2(_SCS_EXPAND2(_SCS_EXPAND2(__VA_ARGS__)))
#define _SCS_EXPAND4(...) _SCS_EXPAND3(_SCS_EXPAND3(_SCS_EXPAND3(__VA_ARGS__)))
#define _SCS_EXPAND5(...) _SCS_EXPAND4(_SCS_EXPAND4(_SCS_EXPAND4(__VA_ARGS__)))
#define _SCS_EXPAND(...)  _SCS_EXPAND5(_SCS_EXPAND5(_SCS_EXPAND5(__VA_ARGS__)))

// Basic macro tools
#define _SCS_FIRST_ARG(first, ...) first
#define _SCS_REST_ARG(first, ...) __VA_ARGS__
#define _SCS_EMPTY()
#define _SCS_DEFER1(m) m _SCS_EMPTY()
#define _SCS_DEFER2(m) m _SCS_EMPTY _SCS_EMPTY()()
#define _SCS_DEFER3(m) m _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY()()()
#define _SCS_DEFER4(m) m _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY()()()()
#define _SCS_DEFER5(m) m _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY _SCS_EMPTY()()()()()

// Conditional macros
#define _SCS_IS_PROBE(...) _SCS_SECOND_ARG(__VA_ARGS__, 0)
#define _SCS_PROBE() ~, 1
#define _SCS_IS_PAREN(x) _SCS_IS_PROBE(_SCS_IS_PAREN_PROBE x)
#define _SCS_IS_PAREN_PROBE(...) _SCS_PROBE()
#define _SCS_SECOND_ARG(_, second, ...) second

#define _SCS_NOT(x) _SCS_IS_PROBE(_SCS_CAT(_SCS_NOT_, x))
#define _SCS_NOT_0 _SCS_PROBE()
#define _SCS_CAT(a, ...) a##__VA_ARGS__
#define _SCS_BOOL(x) _SCS_NOT(_SCS_NOT(x))

#define _SCS_IF_ELSE(condition) _SCS_CAT(_SCS_IF_, condition)
#define _SCS_IF_1(...) __VA_ARGS__ _SCS_IF_1_ELSE
#define _SCS_IF_0(...) _SCS_IF_0_ELSE
#define _SCS_IF_1_ELSE(...)
#define _SCS_IF_0_ELSE(...) __VA_ARGS__

#define _SCS_HAS_ARGS(...) _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ __VA_ARGS__)())
#define _SCS_END_OF_ARGUMENTS_(...) _SCS_IS_PAREN(__VA_ARGS__)

#define _SCS_REMOVE_PARENS(...)            \
  _SCS_IF_ELSE(_SCS_IS_PAREN(__VA_ARGS__)) \
    (_SCS_STRIP_PARENS __VA_ARGS__)        \
    (__VA_ARGS__)

#define _SCS_STRIP_PARENS(...) __VA_ARGS__

#endif // SCS_MACROS_H
