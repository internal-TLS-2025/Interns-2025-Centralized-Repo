#include <iostream>
#include <fstream>
#include "person.pb.h"  // Include the generated header file for Person

// Function to serialize the Person object to a file
void SerializePerson(const Person& person, const std::string& filename) {
    std::ofstream output(filename, std::ios::out | std::ios::binary);
    if (!person.SerializeToOstream(&output)) {
        std::cerr << "Failed to write person to file." << std::endl;
    } else {
        std::cout << "Person serialized successfully to " << filename << std::endl;
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
    // Initialize a Person object with values
    Person person;
    person.set_name("John Doe");
    person.set_id(1234);
    person.set_email("johndoe@example.com");

    // Specify the filename for storing serialized data
    std::string filename = "person_data.bin";

    // Serialize the Person object to the file
    SerializePerson(person, filename);

    // Deserialize the Person object from the file and display its content
    DeserializePerson(filename);

    return 0;
}

