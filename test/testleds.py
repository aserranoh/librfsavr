#!/usr/bin/env python

from autotests import pass_
from avrtests import load_program

LEDS_PROGRAM = "testleds.hex"
SLEEP_TIME = 2

def main():
    load_program(LEDS_PROGRAM)
    pass_()

if __name__ == "__main__":
    main()