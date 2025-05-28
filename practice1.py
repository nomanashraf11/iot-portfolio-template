class Person:
    def __init__(self, name, age, height, weight):
        self.name = name
        self.age = age
        self.height = height
        self.weight = weight

    def bmi(self):
        height_in_meters = self.height / 100
        bmi_value = self.weight / (height_in_meters ** 2)
        return round(bmi_value, 1)

    def weight_status(self):
        bmi_value = self.bmi()
        if bmi_value < 19:
            return "you should gain weight"
        elif bmi_value > 25:
            return "you should lose weight"
        else:
            return "you are of normal weight"


def read_data_from_file(filename):
    people = []
    with open(filename, 'r') as file:
        for line in file:
            name, age, height, weight = line.strip().split(', ')
            person = Person(name, int(age), int(height), int(weight))
            people.append(person)
    return people


def display_person_info(people):
    for person in people:
        bmi_value = person.bmi()
        status = person.weight_status()
        print(f"{person.name}, {person.age} years old: your body mass index is {bmi_value}, {status}")


filename = 'healthcheck.txt'
people = read_data_from_file(filename)
display_person_info(people)
