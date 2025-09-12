#include <iostream>
#include <scs/serializer.h>

using namespace scs;

// Test struct definitions
struct Address {
    std::string street;
    std::string city;
    int32_t zip_code;
    
    SCS_STRUCTURE(Address, street, city, zip_code);
};

struct ContactInfo {
    std::string phone;
    std::string email;
    std::map<int32_t, std::string> phone_extensions;
    
    SCS_STRUCTURE(ContactInfo, phone, email, phone_extensions);
};

struct Person {
    int32_t id;
    std::string name;
    double salary;
    std::vector<int32_t> scores;
    Address address;
    ContactInfo contact;
    
    SCS_STRUCTURE(Person, id, name, salary, scores, address, contact);
};

int main() {
    // Initialize test data
    Address address;
    address.street = "123 Main St";
    address.city = "New York";
    address.zip_code = 10001;
    
    ContactInfo contact;
    contact.phone = "555-1234";
    contact.email = "alice@example.com";
    contact.phone_extensions = {
        {101, "Sales"},
        {102, "Support"},
        {103, "HR"}
    };
    
    Person person;
    person.id = 123;
    person.name = "Alice";
    person.salary = 50000.50;
    person.scores = {95, 88, 92, 78};
    person.address = address;
    person.contact = contact;
    
    // Print original object
    std::cout << "Original object:\n";
    Serializer<Person>::debug_print(person);
    
    // Serialize
    std::vector<char> buffer = Serializer<Person>::serialize(person);
    print_buffer(buffer);
    
    // Deserialize
    Person deserialized = Serializer<Person>::deserialize(buffer);
    
    // Print deserialized object
    std::cout << "Deserialized object:\n";
    Serializer<Person>::debug_print(deserialized);
    
    return 0;
}