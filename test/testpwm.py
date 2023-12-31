#!/usr/bin/env python

from autotests import pass_, fail
from avrtests import load_program, DEVICE
from serial import Serial
from time import sleep
from typing import Callable

PWM_PROGRAM = "testpwm.hex"
COMM_BAUDS = 19200
SLEEP_TIME = 2
ALL_TESTS_SIZE = 108
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
    for index in range(1):
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

def check_test_set_frequency_exact(clock: int, mode: int, ocra: int) -> Callable[[list[str]], bool]:
    def check_frequency(data: list[str]):
        values = [int(x, base=16) for x in data]
        print(values)
        return (values[0] & CRA_MODE_MASK == mode) and (values[1] & CRB_MODE_MASK == 0b00001000) and (values[1] & CLOCK_MASK == clock) and values[2] == ocra
    return check_frequency

def check_test_set_duty_cycle(ocra: int) -> Callable[[list[str]], bool]:
    def check_duty_cycle(data: list[str]):
        values = [int(x, base=16) for x in data]
        print(values)
        return values[0] == ocra
    return check_duty_cycle

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
    13: check_test_set_frequency(1, PWM_MODE_FAST),
    14: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    15: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    16: check_test_set_frequency(2, PWM_MODE_FAST),
    17: check_test_set_frequency(2, PWM_MODE_FAST),
    18: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    19: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    20: check_test_set_frequency(3, PWM_MODE_FAST),
    21: check_test_set_frequency(3, PWM_MODE_FAST),
    22: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    23: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    24: check_test_set_frequency(4, PWM_MODE_FAST),
    25: check_test_set_frequency(4, PWM_MODE_FAST),
    26: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    27: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    28: check_test_set_frequency(5, PWM_MODE_FAST),
    29: check_test_set_frequency(5, PWM_MODE_FAST),
    30: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    31: check_test_set_frequency(1, PWM_MODE_FAST),
    32: check_test_set_frequency(1, PWM_MODE_FAST),
    33: check_test_set_frequency(1, PWM_MODE_FAST),
    34: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    35: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    36: check_test_set_frequency(2, PWM_MODE_FAST),
    37: check_test_set_frequency(2, PWM_MODE_FAST),
    38: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    39: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    40: check_test_set_frequency(3, PWM_MODE_FAST),
    41: check_test_set_frequency(3, PWM_MODE_FAST),
    42: check_test_set_frequency(4, PWM_MODE_FAST),
    43: check_test_set_frequency(4, PWM_MODE_FAST),
    44: check_test_set_frequency(5, PWM_MODE_FAST),
    45: check_test_set_frequency(5, PWM_MODE_FAST),
    46: check_test_set_frequency(6, PWM_MODE_FAST),
    47: check_test_set_frequency(6, PWM_MODE_FAST),
    48: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    49: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    50: check_test_set_frequency(7, PWM_MODE_FAST),
    51: check_test_set_frequency(7, PWM_MODE_FAST),
    52: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),
    53: check_test_set_frequency_exact(1, PWM_MODE_FAST, 228),
    54: check_test_set_frequency_exact(1, PWM_MODE_FAST, 255),
    55: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 128),
    56: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 255),
    57: check_test_set_frequency_exact(2, PWM_MODE_FAST, 64),
    58: check_test_set_frequency_exact(2, PWM_MODE_FAST, 255),
    59: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 128),
    60: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 255),
    61: check_test_set_frequency_exact(3, PWM_MODE_FAST, 64),
    62: check_test_set_frequency_exact(3, PWM_MODE_FAST, 255),
    63: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 128),
    64: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 255),
    65: check_test_set_frequency_exact(4, PWM_MODE_FAST, 128),
    66: check_test_set_frequency_exact(4, PWM_MODE_FAST, 255),
    67: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 128),
    68: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 254),
    69: check_test_set_frequency_exact(5, PWM_MODE_FAST, 128),
    70: check_test_set_frequency_exact(5, PWM_MODE_FAST, 252),
    71: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 128),
    72: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 252),
    73: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 255),
    74: check_test_set_frequency_exact(1, PWM_MODE_FAST, 228),
    75: check_test_set_frequency_exact(1, PWM_MODE_FAST, 255),
    76: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 128),
    77: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 255),
    78: check_test_set_frequency_exact(2, PWM_MODE_FAST, 64),
    79: check_test_set_frequency_exact(2, PWM_MODE_FAST, 255),
    80: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 128),
    81: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 255),
    82: check_test_set_frequency_exact(3, PWM_MODE_FAST, 128),
    83: check_test_set_frequency_exact(3, PWM_MODE_FAST, 255),
    84: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 128),
    85: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 255),
    86: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 128),
    87: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 255),
    88: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 128),
    89: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 255),
    90: check_test_set_frequency_exact(6, PWM_MODE_PHASE_CORRECT, 128),
    91: check_test_set_frequency_exact(6, PWM_MODE_PHASE_CORRECT, 254),
    92: check_test_set_frequency_exact(7, PWM_MODE_FAST, 128),
    93: check_test_set_frequency_exact(7, PWM_MODE_FAST, 252),
    94: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 128),
    95: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 252),
    96: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 255),
    97: check_test_set_duty_cycle(0),
    98: check_test_set_duty_cycle(128),
    99: check_test_set_duty_cycle(255),
    100: check_test_set_duty_cycle(0),
    101: check_test_set_duty_cycle(128),
    102: check_test_set_duty_cycle(255),
    103: check_test_set_duty_cycle(0),
    104: check_test_set_duty_cycle(128),
    105: check_test_set_duty_cycle(255),
    106: check_test_set_duty_cycle(0),
    107: check_test_set_duty_cycle(128),
    108: check_test_set_duty_cycle(255),
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