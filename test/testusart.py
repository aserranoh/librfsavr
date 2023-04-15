#!/usr/bin/env python

from avrloader import load, LoadError, ARDUINO_PROGRAMMER
from autotests import get_environment_variable, skip, pass_, fail
from serial import Serial
from time import sleep

USART_PROGRAM = "testusart.hex"
DEVICE_VARIABLE = "AVR_DEV"
ARDUINO_PARTNO = "ATMEGA328P"
PROGRAMMING_BAUDS = 115200
COMM_BAUDS = 19200
SLEEP_TIME = 2

DEVICE = get_environment_variable(DEVICE_VARIABLE)

def test_usart():
    s = Serial(DEVICE, COMM_BAUDS, timeout=1)
    # This sleep is important because the Arduino gets reset when the Serial connection is made
    sleep(SLEEP_TIME)
    message = b"hello, world!\n"
    n = s.write(message)
    received_message = s.readline()
    if (received_message == message):
        pass_()
    else:
        fail()

def main():
    try:
        load(USART_PROGRAM, DEVICE, ARDUINO_PROGRAMMER, ARDUINO_PARTNO, PROGRAMMING_BAUDS)
    except LoadError as e:
        skip()
    test_usart()

if __name__ == "__main__":
    main()