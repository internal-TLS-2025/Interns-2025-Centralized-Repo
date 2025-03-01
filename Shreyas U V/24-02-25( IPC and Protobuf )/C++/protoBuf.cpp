#include <iostream>
#include <fstream>
#include "trial.pb.h"

using namespace std; 

void serializePerson(const string& filename) {
    Person person;
    person.set_name("John Doe");
    person.set_age(22);

    fstream output(filename, ios::out | ios::binary);
    if (!person.SerializeToOstream(&output)) {
        cerr << "Failed to write person." << endl;
        return;
    }
    output.close();
    cout << "Serialized person to " << filename << endl;
}

void deserializePerson(const string& filename) {
    Person person;

    ifstream input(filename, ios::in | ios::binary);
    if (!person.ParseFromIstream(&input)) {
        cerr << "Failed to parse person." << endl;
        return;
    }

    cout << "Deserialized Person:" << endl;
    cout << "Name: " << person.name() << ", Age: " << person.age() << endl;
}

int main() {
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    const string filename = "person.bin";

    serializePerson(filename);

    deserializePerson(filename);

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
