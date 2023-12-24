#!/usr/bin/env python

from autotests import pass_, fail
from avrtests import load_program, DEVICE
from serial import Serial
from time import sleep

PWM_PROGRAM = "testpwm.hex"
COMM_BAUDS = 19200
SLEEP_TIME = 2
ALL_TESTS_SIZE = 1

def check_test_init(data: list[str]) -> bool:
    addresses = [int(x, base=16) for x in data]
    #print(addresses)
    for index in range(6):
        if addresses[2 * index] != addresses[2 * index + 1]:
            return False
    return True

TESTS_CHECKS = {
    1: check_test_init,
    2: check_test_init
}

def check_message_result(message: str) -> tuple[int, bool]:
    message_fields = message.split(":")
    if len(message_fields) != 2:
        return None, None
    test_id, test_data = message_fields
    test_id = int(test_id)
    passed = TESTS_CHECKS[test_id](test_data.replace("\n", "").split(","))
    return test_id, passed

def test_usart() -> None:
    s = Serial(DEVICE, COMM_BAUDS, timeout=1)
    # This sleep is important because the Arduino gets reset when the Serial connection is made
    sleep(SLEEP_TIME)

    executed_tests = 0
    passed_tests = 0

    while executed_tests < ALL_TESTS_SIZE:
        received_message = s.readline()
        test_id, passed = check_message_result(received_message.decode())
        if test_id is not None:
            executed_tests += 1
            if passed:
                passed_tests += 1

    if executed_tests == passed_tests:
        pass_()
    else:
        fail()

def main() -> None:
    load_program(PWM_PROGRAM)
    test_usart()

if __name__ == "__main__":
    main()