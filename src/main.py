class Person:
    def __init__(self, name):
        self.name = name

    def get_name(self):
        return self.name

person = Person("Gerard")

p1 = person.name
p2 = person.get_name()

print(p1)   # Gerard
print(p2)   # Gerard

print("Hello World")
print("Gerard Zennipman")
