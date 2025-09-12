#include <gtest/gtest.h>
#include <scs/serializer.h>

using namespace scs;

TEST(SerializationTest, BasicTypes) {
    std::vector<char> buffer;
    
    // Test int32_t
    int32_t test_int = 42;
    serialize_data(buffer, test_int);
    EXPECT_EQ(buffer.size(), sizeof(int32_t));
    
    // Test double
    double test_double = 3.14159;
    serialize_data(buffer, test_double);
    EXPECT_EQ(buffer.size(), sizeof(int32_t) + sizeof(double));
    
    // Test string
    std::string test_str = "Hello, World!";
    serialize_data(buffer, test_str);
    EXPECT_EQ(buffer.size(), sizeof(int32_t)*2 + sizeof(double) + test_str.size());
}

TEST(SerializationTest, NestedStructures) {
    struct TestInner {
        int32_t a;
        double b;
        SCS_STRUCTURE(TestInner, a, b);
    };
    
    struct TestOuter {
        TestInner inner;
        std::string name;
        SCS_STRUCTURE(TestOuter, inner, name);
    };
    
    TestOuter obj;
    obj.inner.a = 10;
    obj.inner.b = 2.71828;
    obj.name = "Test Object";
    
    auto buffer = Serializer<TestOuter>::serialize(obj);
    EXPECT_GT(buffer.size(), 0);
}