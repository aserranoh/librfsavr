#!/usr/bin/env python

from avrloader import load, LoadError, ARDUINO_PROGRAMMER
from autotests import get_environment_variable, skip, pass_, fail
from time import sleep

LEDS_PROGRAM = "testleds.hex"
DEVICE_VARIABLE = "AVR_DEV"
BAUDS_VARIABLE = "AVR_PROGRAMMING_BAUDS"
ARDUINO_PARTNO = "ATMEGA328P"
SLEEP_TIME = 2

DEVICE = get_environment_variable(DEVICE_VARIABLE)
PROGRAMMING_BAUDS = int(get_environment_variable(BAUDS_VARIABLE))

def main():
    try:
        load(LEDS_PROGRAM, DEVICE, ARDUINO_PROGRAMMER, ARDUINO_PARTNO, PROGRAMMING_BAUDS)
    except LoadError as e:
        skip()
    pass_()

if __name__ == "__main__":
    main()