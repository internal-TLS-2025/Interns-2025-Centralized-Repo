import message_pb2

person = message_pb2.Person()
person.name = "Shreyas"
person.age = 22

serialized_data = person.SerializeToString()
print("Serialized:", serialized_data)

new_person = message_pb2.Person()
new_person.ParseFromString(serialized_data)
print("Deserialized:", new_person.name, new_person.age)

