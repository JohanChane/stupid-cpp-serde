# macro

## `SCS_STRUCTURE`

```cpp
struct Address {
    std::string street;
    std::string city;
    int32_t zip_code;
    
    SCS_STRUCTURE(Address, street, city, zip_code);
};
```

SCS_STRUCTURE 会递归展开成这样:

```cpp
using Members = ::scs::MemberList< \
        ::scs::Member<Address, &Address::street>, \
        ::scs::Member<Address, &Address::city>, \
        ::scs::Member<Address, &Address::zip_code>
    >
```

_SCS_MAP_NEXT_ARGS 会递归调用自己:

```cpp
#define _SCS_MEMBER_FIRST(type, member) \
    ::scs::Member<type, &type::member>

#define _SCS_MAP_NEXT_ARGS(m, args, ...) \
    _SCS_IF_ELSE(_SCS_HAS_ARGS(__VA_ARGS__)) ( \
        , m(_SCS_REMOVE_PARENS(args), _SCS_FIRST_ARG(__VA_ARGS__)) \
        _SCS_DEFER3(__SCS_MAP_NEXT_ARGS)()(m, args, _SCS_REST_ARG(__VA_ARGS__)) \
    )(/* done */)
```

可以理解为:

```cpp
void map_next_args(m, args, ...) {
    if (has_args) {
        // generate: , ::scs::Member<Address, &Address::city>
        m(args, first_arg)      // m: _SCS_MEMBER_FIRST; args: Address
    }
    map_next_args(m, args, rest_args)
}
```

## `_SCS_EXPAND`

```cpp
// Recursive expansion macros
#define _SCS_EXPAND0(...) __VA_ARGS__
#define _SCS_EXPAND1(...) _SCS_EXPAND0(_SCS_EXPAND0(_SCS_EXPAND0(__VA_ARGS__)))
#define _SCS_EXPAND2(...) _SCS_EXPAND1(_SCS_EXPAND1(_SCS_EXPAND1(__VA_ARGS__)))
#define _SCS_EXPAND3(...) _SCS_EXPAND2(_SCS_EXPAND2(_SCS_EXPAND2(__VA_ARGS__)))
#define _SCS_EXPAND4(...) _SCS_EXPAND3(_SCS_EXPAND3(_SCS_EXPAND3(__VA_ARGS__)))
#define _SCS_EXPAND5(...) _SCS_EXPAND4(_SCS_EXPAND4(_SCS_EXPAND4(__VA_ARGS__)))
#define _SCS_EXPAND(...)  _SCS_EXPAND5(_SCS_EXPAND5(_SCS_EXPAND5(__VA_ARGS__)))
```

其他最终结果是 `__VA_ARGS__`, 目的是为了编译解释到宏的最底层表示。比如:

```cpp
#define BAR FOO
#define BAZ BAR
```

编译器可能解析 BAZ 为 BAR, 而不是 FOO。

## `_SCS_DEFER`

使用宏延时展开, 可以防止宏无限递归展开。

```cpp
#define BAD_RECURSE() BAD_RECURSE()         // 会无限展开, 因为编译器展开一次已经递归了。同时编译出错, error: ‘BAD_RECURSE’ was not declared in this scope
```

不会无限展开, 因为编译器要展开两遍才递归:

```cpp
#define GOOD_RECURSE() _SCS_DEFER1(GOOD_RECURSE)()
GOOD_RECURSE()()  // 展开流程：
                  // 1. GOOD_RECURSE () → _SCS_DEFER1(GOOD_RECURSE) () 
                  // 2. GOOD_RECURSE _SCS_EMPTY() () → GOOD_RECURSE () 
                  // 3. 重复上述过程实现递归
```

## `_SCS_IF_ELSE`

用法:

```cpp
_SCS_IF_ELSE(cond)(<cond == 1 才运行的代码>)(<cond == 0 才运行的代码>)
```

## `PROBE`

PROBE: 探针

_SCS_PROBE: 表示一个探针
_SCS_IS_PROBE: 表示是否遇到探针, 如果是, 则返回 1。否, 则返回 0。

```cpp
#define _SCS_SECOND_ARG(_, second, ...) second
#define _SCS_IS_PROBE(...) _SCS_SECOND_ARG(__VA_ARGS__, 0)      // 如果 __VA_ARGS__ 是一个探针, 则返回 1。否则返回 0
#define _SCS_PROBE() ~, 1           // 表示一个探针
```

```cpp
_SCS_SECOND_ARG(, 0)     // → 0
_SCS_IS_PROBE()          // → 0
```

### 检测括号 _SCS_IS_PAREN​​

```cpp
#define _SCS_IS_PAREN(x) _SCS_IS_PROBE(_SCS_IS_PAREN_PROBE x)
#define _SCS_IS_PAREN_PROBE(...) _SCS_PROBE()
```

```cpp
_SCS_IS_PAREN(())     // → _SCS_IS_PROBE(_SCS_IS_PAREN_PROBE ()) 
                      // → _SCS_IS_PROBE(_SCS_PROBE()) → 1          // 实际上有括号会生成一个探针
_SCS_IS_PAREN(abc)    // → _SCS_IS_PROBE(_SCS_IS_PAREN_PROBE abc) 
                      // → _SCS_IS_PROBE(abc /*没有生成一个探针*/) → 0
                      // → _SCS_SECOND_ARG(abc, 0) → 0
_SCS_IS_PAREN()       // → _SCS_IS_PROBE(_SCS_IS_PAREN_PROBE )
                      // → _SCS_IS_PROBE()
                      // → _SCS_SECOND_ARG(, 0)
                      // → 0
```

### 布尔逻辑系统​​

NOT:

```cpp
#define _SCS_NOT(x) _SCS_IS_PROBE(_SCS_CAT(_SCS_NOT_, x))
#define _SCS_NOT_0 _SCS_PROBE()    // 只有_NOT_0会触发探测
#define _SCS_CAT(a, ...) a##__VA_ARGS__
```

```cpp
_SCS_NOT(0)     // → 1
_SCS_NOT(1)     // → _SCS_IS_PROBE(_SCS_CAT(_SCS_NOT_, 1))  
                // → _SCS_IS_PROBE(_SCS_NOT_1)             // 拼接结果为 _SCS_NOT_1（未定义）
                // → _SCS_IS_PROBE()                       // 未定义的宏被替换为空
                // → _SCS_SECOND_ARG(, 0)                  // _SCS_IS_PROBE 填充默认参数
                // → 0
_SCS_NOT(abc)   // → 0
_SCS_NOT()      // → 0
```

BOOL (非 0 即真):

```cpp
#define _SCS_BOOL(x) _SCS_NOT(_SCS_NOT(x))
```

```
_SCS_BOOL(0) → _SCS_NOT(1) → 0
_SCS_BOOL(1) → _SCS_NOT(0) → 1
_SCS_BOOL(9) -> _SCS_NOT(0) -> 1
_SCS_BOOL(a) -> _SCS_NOT(0) -> 1
_SCS_BOOL() -> _SCS_NOT(0) -> 1
```

## `_SCS_HAS_ARGS`

_SCS_END_OF_ARGUMENTS_:

```cpp
#define _SCS_END_OF_ARGUMENTS_(...) _SCS_IS_PAREN(__VA_ARGS__)
```

```cpp
_SCS_END_OF_ARGUMENTS_()        // → 0
_SCS_END_OF_ARGUMENTS_(())      // → 1
```

_SCS_HAS_ARGS (无参数则返回 0, 有参数则返回 1):

```cpp
#define _SCS_HAS_ARGS(...) _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ __VA_ARGS__)())
```

```cpp
_SCS_HAS_ARGS()         // → _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ )())
                        // → _SCS_BOOL(_SCS_END_OF_ARGUMENTS_())
                        // → _SCS_BOOL(0)

_SCS_HAS_ARGS(())       // → _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ ())())
                        // → _SCS_BOOL(_SCS_FIRST_ARG(0)())
                        // → _SCS_BOOL(0())
                        // → 0
                        
_SCS_HAS_ARGS(a)        // → _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ a)())
                        // → _SCS_BOOL(_SCS_END_OF_ARGUMENTS_ a())
                        // → _SCS_BOOL(a())
                        // → 1

_SCS_HAS_ARGS(a, b)     // → _SCS_BOOL(_SCS_FIRST_ARG(_SCS_END_OF_ARGUMENTS_ a, b)())
                        // → _SCS_BOOL(_SCS_END_OF_ARGUMENTS_ a())
                        // → _SCS_BOOL(a())
                        // → 1

_SCS_HAS_ARGS((a, b))     // 编译不通过, 因为 _SCS_IS_PAREN 只接收一个参数
```

## Test macro



```cpp
#include <scs/macros.h>

int main() {
    int i = _SCS_NOT(1);
    return 0;
}
```

```sh
g++ -E main.cc -o main_preprocessed.cc
```