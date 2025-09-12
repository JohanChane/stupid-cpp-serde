#include <gtest/gtest.h>
#include <scs/serializer.h>

using namespace scs;

TEST(SerializationTest, BasicTypes) {
    std::vector<char> buffer;
    
    // Test int32_t
    int32_t test_int = 42;
    Encoding<int32_t>::write_payload(buffer, test_int);
    EXPECT_EQ(buffer.size(), sizeof(int32_t));
    
    // Test double
    double test_double = 3.14159;
    Encoding<double>::write_payload(buffer, test_double);
    EXPECT_EQ(buffer.size(), sizeof(int32_t) + sizeof(double));
    
    // Test string
    std::string test_str = "Hello, World!";
    Encoding<std::string>::write_payload(buffer, test_str);
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
    
    // Additional verification: deserialize and compare
    TestOuter deserialized = Serializer<TestOuter>::deserialize(buffer);
    EXPECT_EQ(obj.inner.a, deserialized.inner.a);
    EXPECT_DOUBLE_EQ(obj.inner.b, deserialized.inner.b);
    EXPECT_EQ(obj.name, deserialized.name);
}

TEST(SerializationTest, ContainerTypes) {
    std::vector<char> buffer;
    
    // Test vector<int>
    std::vector<int> test_vec = {1, 2, 3, 4, 5};
    Encoding<std::vector<int>>::write_payload(buffer, test_vec);
    EXPECT_EQ(buffer.size(), sizeof(SER_DATA_LEN_T) + test_vec.size() * sizeof(int));
    
    // Test vector<string>
    std::vector<std::string> test_str_vec = {"a", "bb", "ccc"};
    Encoding<std::vector<std::string>>::write_payload(buffer, test_str_vec);
    
    // Verify deserialization
    const char* data = buffer.data();
    auto deserialized_vec = Encoding<std::vector<int>>::read_payload(data);
    auto deserialized_str_vec = Encoding<std::vector<std::string>>::read_payload(data);
    
    EXPECT_EQ(test_vec, deserialized_vec);
    EXPECT_EQ(test_str_vec, deserialized_str_vec);
}