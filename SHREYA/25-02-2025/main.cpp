#include <iostream>
#include "person.pb.h"

int main() {
    // Initialize the Person message
    Person person;

    // Set values for the fields
    person.set_name("John Doe");
    person.set_id(1234);
    person.set_email("johndoe@example.com");

    // Print out the values of the Person object
    std::cout << "Name: " << person.name() << std::endl;
    std::cout << "ID: " << person.id() << std::endl;
    std::cout << "Email: " << person.email() << std::endl;

    // Serialize the object to a string (can be written to a file or network)
    std::string serialized_data;
    person.SerializeToString(&serialized_data);
    std::cout << "Serialized data size: " << serialized_data.size() << " bytes" << std::endl;

    // Deserialize the data back into a new Person object
    Person new_person;
    if (new_person.ParseFromString(serialized_data)) {
        std::cout << "Deserialized Person:" << std::endl;
        std::cout << "Name: " << new_person.name() << std::endl;
        std::cout << "ID: " << new_person.id() << std::endl;
        std::cout << "Email: " << new_person.email() << std::endl;
    } else {
        std::cerr << "Failed to parse the serialized data." << std::endl;
    }

    return 0;
}
