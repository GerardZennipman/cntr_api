import ctypes
import time

class Person:
    def __init__(self, name):
        self.name = name

    def get_name(self):
        return self.name

class Led:
    def __init__(self, gpio, pin):
        self.gpio = gpio
        self.pin = pin

    def toggle_nrof_times(self, nrof_times):
        for loop_nr in range(nrof_times):
            print(f"gpio{self.pin} (green led) = 1")
            self.gpio.gpio_set(self.pin, 1)
            time.sleep(1.0)

            print(f"gpio{self.pin} (green led) = 0")
            self.gpio.gpio_set(self.pin, 0)
            time.sleep(1.0)

# Create a Person object
person = Person("Gerard")

p1 = person.name
p2 = person.get_name()

print(p1)
print(p2)

print("Hello World")
print("Gerard Zennipman")

# Load the dynamically linked library
gpio = ctypes.CDLL("./libgpio.so")

gpio.gpio_init.argtypes = []
gpio.gpio_init.restype = None

gpio.gpio_set.argtypes = [
    ctypes.c_uint32,
    ctypes.c_uint32
]
gpio.gpio_set.restype = None

gpio.gpio_init()

# Create a Led object
green_led = Led(gpio, 14)

# Toggle the LED four times
green_led.toggle_nrof_times(4)
