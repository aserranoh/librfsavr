#!/usr/bin/env python

from autotests import pass_, fail
from avrtests import load_program, DEVICE
from serial import Serial
from time import sleep
from typing import Callable

PWM_PROGRAM = "testpwm.hex"
COMM_BAUDS = 19200
SLEEP_TIME = 2
ALL_TESTS_SIZE = 52
COMA_MASK = 0b11000000
COMB_MASK = 0b00110000
DDRD6_MASK = 0b01000000
DDRD5_MASK = 0b00100000
DDRB3_MASK = 0b00001000
DDRD3_MASK = 0b00001000
CRA_MODE_MASK = 0b00000011
CRB_MODE_MASK = 0b00001000
CLOCK_MASK = 0b00000111
PWM_MODE_FAST = 3
PWM_MODE_PHASE_CORRECT = 1

def check_test_init(data: list[str]) -> bool:
    addresses = [int(x, base=16) for x in data]
    print(addresses)
    for index in range(6):
        if addresses[2 * index] != addresses[2 * index + 1]:
            return False
    return True

def check_test_enable_channel_A_timer0(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMA_MASK == 0b10000000) and (values[1] & DDRD6_MASK == DDRD6_MASK)

def check_test_enable_channel_B_timer0(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMB_MASK == 0b00100000) and (values[1] & DDRD5_MASK == DDRD5_MASK)

def check_test_enable_channel_A_timer2(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMA_MASK == 0b10000000) and (values[1] & DDRB3_MASK == DDRB3_MASK)

def check_test_enable_channel_B_timer2(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMB_MASK == 0b00100000) and (values[1] & DDRD3_MASK == DDRD3_MASK)

def check_test_disable_channel_A(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMA_MASK == 0)

def check_test_disable_channel_B(data: list[str]) -> bool:
    values = [int(x, base=16) for x in data]
    print(values)
    return (values[0] & COMB_MASK == 0)

def check_test_set_frequency(clock: int, mode: int) -> Callable[[list[str]], bool]:
    def check_frequency(data: list[str]):
        values = [int(x, base=16) for x in data]
        print(values)
        return (values[0] & CRA_MODE_MASK == mode) and (values[1] & CRB_MODE_MASK == 0) and (values[1] & CLOCK_MASK == clock)
    return check_frequency

TESTS_CHECKS = {
    1: check_test_init,
    2: check_test_init,
    3: check_test_enable_channel_A_timer0,
    4: check_test_enable_channel_B_timer0,
    5: check_test_enable_channel_A_timer2,
    6: check_test_enable_channel_B_timer2,
    7: check_test_disable_channel_A,
    8: check_test_disable_channel_B,
    9: check_test_disable_channel_A,
    10: check_test_disable_channel_B,
    11: check_test_set_frequency(1, PWM_MODE_FAST),
    12: check_test_set_frequency(1, PWM_MODE_FAST),
    13: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    14: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    15: check_test_set_frequency(2, PWM_MODE_FAST),
    16: check_test_set_frequency(2, PWM_MODE_FAST),
    17: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    18: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    19: check_test_set_frequency(3, PWM_MODE_FAST),
    20: check_test_set_frequency(3, PWM_MODE_FAST),
    21: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    22: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    23: check_test_set_frequency(4, PWM_MODE_FAST),
    24: check_test_set_frequency(4, PWM_MODE_FAST),
    25: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    26: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    27: check_test_set_frequency(5, PWM_MODE_FAST),
    28: check_test_set_frequency(5, PWM_MODE_FAST),
    29: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    30: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    31: check_test_set_frequency(1, PWM_MODE_FAST),
    32: check_test_set_frequency(1, PWM_MODE_FAST),
    33: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    34: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    35: check_test_set_frequency(2, PWM_MODE_FAST),
    36: check_test_set_frequency(2, PWM_MODE_FAST),
    37: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    38: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    39: check_test_set_frequency(3, PWM_MODE_FAST),
    40: check_test_set_frequency(3, PWM_MODE_FAST),
    41: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    42: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    43: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    44: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    45: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    46: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    47: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    48: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    49: check_test_set_frequency(7, PWM_MODE_FAST),
    50: check_test_set_frequency(7, PWM_MODE_FAST),
    51: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),
    52: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),
}

def check_message_result(message: str) -> tuple[int, bool]:
    message_fields = message.split(":")
    if len(message_fields) != 2:
        return None, None
    test_id, test_data = message_fields
    test_id = int(test_id)
    passed = TESTS_CHECKS[test_id](test_data.replace("\n", "").split(","))
    print(f"test {test_id}: {'PASS' if passed else 'FAIL'}")
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