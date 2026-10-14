import pytest
from src.main import Person

def test_person_get_name():
    person = Person("Marion")
    assert person.get_name() == "Marion"
