#include <gtest/gtest.h>
#include <scs/base/members.h>
#include <scs/serializer.h>
#include <type_traits>

using namespace scs;

// Helper macro to access member types
#define MEMBER_TYPE(N, Members) typename std::tuple_element_t<N, typename Members::Tuple>::MemberType

TEST(StructureTest, MemberReflection) {
    struct TestStruct {
        int32_t a;
        double b;
        std::string c;
        SCS_STRUCTURE(TestStruct, a, b, c);
    };

    // Verify member count
    constexpr size_t expected_members = 3;
    EXPECT_EQ(TestStruct::Members::size, expected_members);

    // Verify member types
    EXPECT_TRUE((std::is_same_v<MEMBER_TYPE(0, TestStruct::Members), int32_t>));
    EXPECT_TRUE((std::is_same_v<MEMBER_TYPE(1, TestStruct::Members), double>));
    EXPECT_TRUE((std::is_same_v<MEMBER_TYPE(2, TestStruct::Members), std::string>));
}

TEST(StructureTest, NestedStructureReflection) {
    struct Inner {
        int x;
        double y;
        SCS_STRUCTURE(Inner, x, y);
    };

    struct Outer {
        Inner inner;
        std::string name;
        SCS_STRUCTURE(Outer, inner, name);
    };

    // Verify outer structure member count
    constexpr size_t outer_members = 2;
    EXPECT_EQ(Outer::Members::size, outer_members);

    // Verify inner structure reflection
    EXPECT_TRUE(has_members_v<Inner>);

    // Verify serialization/deserialization
    Outer original;
    original.inner.x = 10;
    original.inner.y = 3.14;
    original.name = "Nested";

    auto buffer = Serializer<Outer>::serialize(original);
    auto deserialized = Serializer<Outer>::deserialize(buffer);

    EXPECT_EQ(original.inner.x, deserialized.inner.x);
    EXPECT_DOUBLE_EQ(original.inner.y, deserialized.inner.y);
    EXPECT_EQ(original.name, deserialized.name);
}

TEST(StructureTest, EmptyStructure) {
    struct Empty {
        SCS_STRUCTURE(Empty);
    };

    struct WithEmpty {
        int a;
        Empty empty;
        SCS_STRUCTURE(WithEmpty, a, empty);
    };

    WithEmpty original;
    original.a = 42;

    auto buffer = Serializer<WithEmpty>::serialize(original);
    auto deserialized = Serializer<WithEmpty>::deserialize(buffer);

    EXPECT_EQ(original.a, deserialized.a);
}

TEST(StructureTest, InheritedStructure) {
    struct Base {
        int base_val;
        SCS_STRUCTURE(Base, base_val);
    };

    struct Derived : Base {
        std::string derived_val;
        SCS_STRUCTURE(Derived, base_val, derived_val);
    };

    Derived original;
    original.base_val = 100;
    original.derived_val = "inherited";

    auto buffer = Serializer<Derived>::serialize(original);
    auto deserialized = Serializer<Derived>::deserialize(buffer);

    EXPECT_EQ(original.base_val, deserialized.base_val);
    EXPECT_EQ(original.derived_val, deserialized.derived_val);
}