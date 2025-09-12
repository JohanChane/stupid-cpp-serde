#include <gtest/gtest.h>
#include <scs/serializer.h>
#include <vector>
#include <string>

using namespace scs;

// Test deserialization of primitive types
TEST(DeserializationTest, PrimitiveTypes) {
    std::vector<char> buffer;
    
    // Serialize test data
    int32_t original_int = 42;
    double original_double = 3.14159;
    serialize_data(buffer, original_int);
    serialize_data(buffer, original_double);

    // Deserialize
    const char* data = buffer.data();
    auto deserialized_int = deserialize_data<int32_t>(data);
    auto deserialized_double = deserialize_data<double>(data);

    EXPECT_EQ(original_int, deserialized_int);
    EXPECT_DOUBLE_EQ(original_double, deserialized_double);
}

// Test string deserialization
TEST(DeserializationTest, StringType) {
    std::vector<char> buffer;
    std::string original = "Hello, SCS!";
    
    serialize_data(buffer, original);

    const char* data = buffer.data();
    auto deserialized = deserialize_data<std::string>(data);

    EXPECT_EQ(original, deserialized);
}

// Test nested structure deserialization
TEST(DeserializationTest, NestedStructure) {
    struct Address {
        std::string street;
        int zip_code;
        SCS_STRUCTURE(Address, street, zip_code);
    };

    struct Person {
        std::string name;
        Address address;
        SCS_STRUCTURE(Person, name, address);
    };

    // Prepare test data
    Person original;
    original.name = "Alice";
    original.address.street = "123 Main St";
    original.address.zip_code = 10001;

    // Serialize
    auto buffer = Serializer<Person>::serialize(original);

    // Deserialize
    Person deserialized = Serializer<Person>::deserialize(buffer);

    // Verify
    EXPECT_EQ(original.name, deserialized.name);
    EXPECT_EQ(original.address.street, deserialized.address.street);
    EXPECT_EQ(original.address.zip_code, deserialized.address.zip_code);
}

// Test container deserialization
TEST(DeserializationTest, ContainerTypes) {
    std::vector<char> buffer;
    std::vector<int> original_vec = {1, 2, 3, 4, 5};
    std::vector<std::string> original_str_vec = {"a", "bb", "ccc"};

    serialize_data(buffer, original_vec);
    serialize_data(buffer, original_str_vec);

    const char* data = buffer.data();
    auto deserialized_vec = deserialize_data<std::vector<int>>(data);
    auto deserialized_str_vec = deserialize_data<std::vector<std::string>>(data);

    EXPECT_EQ(original_vec, deserialized_vec);
    EXPECT_EQ(original_str_vec, deserialized_str_vec);
}