#include <iostream>
   #include "exmple.pb.h"

   int main() {
       example::Person person;
       person.set_name("Alice");
       person.set_id(123);

       // Serialize the object to a string
       std::string serialized_data;
       if (!person.SerializeToString(&serialized_data)) {
           std::cerr << "Failed to serialize person." << std::endl;
           return 1;
       }

       std::cout << "Serialized person data: " << serialized_data << std::endl;

       // Deserialize the object from the string
       example::Person new_person;
       if (!new_person.ParseFromString(serialized_data)) {
           std::cerr << "Failed to parse person." << std::endl;
           return 1;
       }

       std::cout << "Deserialized Person: " << new_person.name() << " (ID: " << new_person.id() << ")" << std::endl;

       return 0;
   }
