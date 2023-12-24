
from avrloader import Loader, LoadError
from autotests import get_environment_variable, skip

DEVICE_VARIABLE = "AVR_DEV"
BAUDS_VARIABLE = "AVR_PROGRAMMING_BAUDS"
AVR_PARTNO = "ATMEGA328P"

DEVICE = get_environment_variable(DEVICE_VARIABLE)
PROGRAMMING_BAUDS = int(get_environment_variable(BAUDS_VARIABLE))

def load_program(program_file: str) -> None:
    loader = Loader(device=DEVICE, bauds=PROGRAMMING_BAUDS)
    try:
        loader.load(program_file, AVR_PARTNO)
    except LoadError as e:
        skip()