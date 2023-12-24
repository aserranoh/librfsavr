#!/usr/bin/env python

from avrloader import Loader, LoadError
from autotests import pass_, fail
from avrtests import load_program, DEVICE
from serial import Serial
from time import sleep

USART_PROGRAM = "testusart.hex"
COMM_BAUDS = 19200
SLEEP_TIME = 2

def test_usart() -> None:
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

def main() -> None:
    load_program(USART_PROGRAM)
    test_usart()

if __name__ == "__main__":
    main()