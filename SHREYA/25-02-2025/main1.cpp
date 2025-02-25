#include <iostream>
#include <fstream>
#include "person.pb.h"  // Include the generated header file for Person

// Function to serialize the Person object to a file
void SerializePerson(const Person& person, const std::string& filename) {
    std::ofstream output(filename, std::ios::out | std::ios::binary);
    if (!person.SerializeToOstream(&output)) {
        std::cerr << "Failed to write person to file." << std::endl;
    }
    output.close();
}

// Function to deserialize the Person object from a file
void DeserializePerson(const std::string& filename) {
    Person person;
    std::ifstream input(filename, std::ios::in | std::ios::binary);
    if (!person.ParseFromIstream(&input)) {
        std::cerr << "Failed to read person from file." << std::endl;
    } else {
        std::cout << "Deserialized Person:" << std::endl;
        std::cout << "Name: " << person.name() << std::endl;
        std::cout << "ID: " << person.id() << std::endl;
        std::cout << "Email: " << person.email() << std::endl;
    }
    input.close();
}

int main() {
    // Initialize a Person object
    Person person;
    person.set_name("John Doe");
    person.set_id(1234);
    person.set_email("johndoe@example.com");

    // Serialize to file
    std::string filename = "person_data.bin";
    SerializePerson(person, filename);

    // Deserialize from file
    DeserializePerson(filename);

    return 0;
}
