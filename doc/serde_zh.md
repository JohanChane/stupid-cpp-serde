# Serialization and Deserialization

## Binary Format

为了使代码简单方便阅读, 采用了一种很简单且没有实际意义的编码方案。

对于非 `is_arithmetic` 数据类型, 比如: string 、vector 和 struct 等 使用 "数据长度字段(固定长度, 类型为 SER_DATA_LEN_T) + 实际数据字段"

而 `is_arithmetic` (char, int, float, ...) 数据类型则没有数据长度字段, 只有实际数据字段。

[参考](../include/scs/serializer.h)

## 数据类型的编译信息

通过 `SCS_STRUCTURE` 维护序列化与反序列时需要的数据类型的编译信息。

比如:

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

Member 对应一个成员的编译信息。MemberList 的参数可以通过递归模板实例化来取出每个模板参数, 这样就可以按递归的方式遍历成员了。

序列化从 Member::value 中取出数据。反序列化时, 将恢复的数据放到 value 的位置即可。