#!/usr/bin/env python

from autotests import pass_, fail
from avrtests import load_program, DEVICE
from serial import Serial
from time import sleep
from typing import Callable

PWM_PROGRAM = "testpwm.hex"
COMM_BAUDS = 19200
SLEEP_TIME = 2
ALL_TESTS_SIZE = 147
CHANNEL_A = 0
CHANNEL_B = 1
OUTPUT_PIN_TIMER0_CHANNEL_A = 6
OUTPUT_PIN_TIMER0_CHANNEL_B = 5
OUTPUT_PIN_TIMER1_CHANNEL_A = 1
OUTPUT_PIN_TIMER1_CHANNEL_B = 2
OUTPUT_PIN_TIMER2_CHANNEL_A = 3
OUTPUT_PIN_TIMER2_CHANNEL_B = 3
COMA_MASK = 0b11000000
COMB_MASK = 0b00110000
DDRD6_MASK = 0b01000000
DDRD5_MASK = 0b00100000
DDRB1_MASK = 0b00000010
DDRB2_MASK = 0b00000100
DDRB3_MASK = 0b00001000
DDRD3_MASK = 0b00001000
CRA_MODE_MASK = 0b00000011
CRB_MODE_MASK = 0b00011000
CLOCK_MASK = 0b00000111
PWM_MODE_FAST = 3
PWM_MODE_PHASE_CORRECT = 1
PWM_MODE_FAST_8 = 0b101
PWM_MODE_FAST_9 = 0b110
PWM_MODE_FAST_10 = 0b111
PWM_MODE_PHASE_CORRECT_8 = 0b001
PWM_MODE_PHASE_CORRECT_9 = 0b010
PWM_MODE_PHASE_CORRECT_10 = 0b011

def get_values(data: list[str]) -> list[int]:
    values = [int(x, base=16) for x in data]
    print([hex(x) for x in values])
    return values

def check_test_init(channel: int, pin: int) -> Callable[[list[str]], bool]:
    def check_init(data: list[str]) -> bool:
        values = get_values(data)
        for index in range(5):
            if values[2 * index] != values[2 * index + 1]:
                return False
        if channel == CHANNEL_A:
            ok = (values[-1] & COMA_MASK == 0b10000000)
        else:
            ok = (values[-1] & COMB_MASK == 0b00100000)
        return ok and values[-3] == channel and values[-2] == pin
    return check_init

def check_test_close(channel: int) -> Callable[[list[str]], bool]:
    def check_close(data: list[str]) -> bool:
        values = get_values(data)
        if channel == CHANNEL_A:
            ok = (values[0] & COMA_MASK == 0)
        else:
            ok = (values[0] & COMB_MASK == 0)
        return ok and (values[0] & CRA_MODE_MASK == 0) and (values[1] & CRB_MODE_MASK == 0) and (values[1] & CLOCK_MASK == 0)
    return check_close

def check_test_set_frequency(clock: int, mode: int) -> Callable[[list[str]], bool]:
    def check_frequency(data: list[str]):
        values = get_values(data)
        return (values[0] & CRA_MODE_MASK == mode) and (values[1] & CRB_MODE_MASK == 0) and (values[1] & CLOCK_MASK == clock)
    return check_frequency

def check_test_set_frequency_16(clock: int, mode: int) -> Callable[[list[str]], bool]:
    def check_frequency(data: list[str]):
        values = get_values(data)
        read_mode = (values[0] & CRA_MODE_MASK) | ((values[1] & CRB_MODE_MASK) >> 1)
        return (read_mode == mode) and (values[1] & CLOCK_MASK == clock)
    return check_frequency

def check_test_set_frequency_exact(clock: int, mode: int, ocra: int) -> Callable[[list[str]], bool]:
    def check_frequency(data: list[str]):
        values = get_values(data)
        return (values[0] & CRA_MODE_MASK == mode) and (values[1] & CRB_MODE_MASK == 0b00001000) and (values[1] & CLOCK_MASK == clock) and values[2] == ocra
    return check_frequency

def check_test_set_frequency_exact_16(clock: int, mode: int, ocra: int) -> Callable[[list[str]], bool]:
    mode = (0b11000 if mode == PWM_MODE_FAST else 0b10000)
    def check_frequency(data: list[str]):
        values = get_values(data)
        return (values[0] & CRA_MODE_MASK == 2) and (values[1] & CRB_MODE_MASK == mode) and (values[1] & CLOCK_MASK == clock) and values[2] == ocra
    return check_frequency

def check_test_set_duty_cycle(ocra: int) -> Callable[[list[str]], bool]:
    def check_duty_cycle(data: list[str]):
        values = get_values(data)
        return values[0] == ocra
    return check_duty_cycle

TESTS_CHECKS = {
    1: check_test_init(CHANNEL_A, OUTPUT_PIN_TIMER0_CHANNEL_A),
    2: check_test_init(CHANNEL_B, OUTPUT_PIN_TIMER0_CHANNEL_B),
    3: check_test_init(CHANNEL_A, OUTPUT_PIN_TIMER1_CHANNEL_A),
    4: check_test_init(CHANNEL_B, OUTPUT_PIN_TIMER1_CHANNEL_B),
    5: check_test_init(CHANNEL_A, OUTPUT_PIN_TIMER2_CHANNEL_A),
    6: check_test_init(CHANNEL_B, OUTPUT_PIN_TIMER2_CHANNEL_B),
    7: check_test_close(CHANNEL_A),
    8: check_test_close(CHANNEL_B),
    9: check_test_close(CHANNEL_A),
    10: check_test_close(CHANNEL_B),
    11: check_test_close(CHANNEL_A),
    12: check_test_close(CHANNEL_B),
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
    31: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    32: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    33: check_test_set_frequency(1, PWM_MODE_FAST),
    34: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    35: check_test_set_frequency(1, PWM_MODE_PHASE_CORRECT),
    36: check_test_set_frequency(2, PWM_MODE_FAST),
    37: check_test_set_frequency(2, PWM_MODE_FAST),
    38: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    39: check_test_set_frequency(2, PWM_MODE_PHASE_CORRECT),
    40: check_test_set_frequency(3, PWM_MODE_FAST),
    41: check_test_set_frequency(3, PWM_MODE_FAST),
    42: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    43: check_test_set_frequency(3, PWM_MODE_PHASE_CORRECT),
    44: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    45: check_test_set_frequency(4, PWM_MODE_PHASE_CORRECT),
    46: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    47: check_test_set_frequency(5, PWM_MODE_PHASE_CORRECT),
    48: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    49: check_test_set_frequency(6, PWM_MODE_PHASE_CORRECT),
    50: check_test_set_frequency(7, PWM_MODE_FAST),
    51: check_test_set_frequency(7, PWM_MODE_FAST),
    52: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),
    53: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),
    54: check_test_set_frequency(7, PWM_MODE_PHASE_CORRECT),

    55: check_test_set_frequency_16(1, PWM_MODE_FAST_8),
    56: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_8),
    57: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_8),
    58: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_9),
    59: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_9),
    60: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_10),
    61: check_test_set_frequency_16(1, PWM_MODE_PHASE_CORRECT_10),
    62: check_test_set_frequency_16(2, PWM_MODE_FAST_10),
    63: check_test_set_frequency_16(2, PWM_MODE_FAST_10),
    64: check_test_set_frequency_16(2, PWM_MODE_PHASE_CORRECT_10),
    65: check_test_set_frequency_16(2, PWM_MODE_PHASE_CORRECT_10),
    66: check_test_set_frequency_16(3, PWM_MODE_FAST_10),
    67: check_test_set_frequency_16(3, PWM_MODE_FAST_10),
    68: check_test_set_frequency_16(3, PWM_MODE_PHASE_CORRECT_10),
    69: check_test_set_frequency_16(3, PWM_MODE_PHASE_CORRECT_10),
    70: check_test_set_frequency_16(4, PWM_MODE_FAST_10),
    71: check_test_set_frequency_16(4, PWM_MODE_FAST_10),
    72: check_test_set_frequency_16(4, PWM_MODE_PHASE_CORRECT_10),
    73: check_test_set_frequency_16(4, PWM_MODE_PHASE_CORRECT_10),
    74: check_test_set_frequency_16(5, PWM_MODE_FAST_10),
    75: check_test_set_frequency_16(5, PWM_MODE_FAST_10),
    76: check_test_set_frequency_16(5, PWM_MODE_PHASE_CORRECT_10),
    77: check_test_set_frequency_16(5, PWM_MODE_PHASE_CORRECT_10),
    78: check_test_set_frequency_16(5, PWM_MODE_PHASE_CORRECT_10),

    79: check_test_set_frequency_exact(1, PWM_MODE_FAST, 228),
    80: check_test_set_frequency_exact(1, PWM_MODE_FAST, 255),
    81: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 128),
    82: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 255),
    83: check_test_set_frequency_exact(2, PWM_MODE_FAST, 64),
    84: check_test_set_frequency_exact(2, PWM_MODE_FAST, 255),
    85: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 128),
    86: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 255),
    87: check_test_set_frequency_exact(3, PWM_MODE_FAST, 64),
    88: check_test_set_frequency_exact(3, PWM_MODE_FAST, 255),
    89: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 128),
    90: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 255),
    91: check_test_set_frequency_exact(4, PWM_MODE_FAST, 128),
    92: check_test_set_frequency_exact(4, PWM_MODE_FAST, 255),
    93: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 128),
    94: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 254),
    95: check_test_set_frequency_exact(5, PWM_MODE_FAST, 128),
    96: check_test_set_frequency_exact(5, PWM_MODE_FAST, 252),
    97: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 128),
    98: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 252),
    99: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 255),
    100: check_test_set_frequency_exact(1, PWM_MODE_FAST, 228),
    101: check_test_set_frequency_exact(1, PWM_MODE_FAST, 255),
    102: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 128),
    103: check_test_set_frequency_exact(1, PWM_MODE_PHASE_CORRECT, 255),
    104: check_test_set_frequency_exact(2, PWM_MODE_FAST, 64),
    105: check_test_set_frequency_exact(2, PWM_MODE_FAST, 255),
    106: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 128),
    107: check_test_set_frequency_exact(2, PWM_MODE_PHASE_CORRECT, 255),
    108: check_test_set_frequency_exact(3, PWM_MODE_FAST, 128),
    109: check_test_set_frequency_exact(3, PWM_MODE_FAST, 255),
    110: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 128),
    111: check_test_set_frequency_exact(3, PWM_MODE_PHASE_CORRECT, 255),
    112: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 128),
    113: check_test_set_frequency_exact(4, PWM_MODE_PHASE_CORRECT, 255),
    114: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 128),
    115: check_test_set_frequency_exact(5, PWM_MODE_PHASE_CORRECT, 255),
    116: check_test_set_frequency_exact(6, PWM_MODE_PHASE_CORRECT, 128),
    117: check_test_set_frequency_exact(6, PWM_MODE_PHASE_CORRECT, 254),
    118: check_test_set_frequency_exact(7, PWM_MODE_FAST, 128),
    119: check_test_set_frequency_exact(7, PWM_MODE_FAST, 252),
    120: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 128),
    121: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 252),
    122: check_test_set_frequency_exact(7, PWM_MODE_PHASE_CORRECT, 255),
    123: check_test_set_frequency_exact_16(1, PWM_MODE_FAST, 53333),
    124: check_test_set_frequency_exact_16(1, PWM_MODE_FAST, 65306),
    125: check_test_set_frequency_exact_16(1, PWM_MODE_PHASE_CORRECT, 32786),
    126: check_test_set_frequency_exact_16(1, PWM_MODE_PHASE_CORRECT, 65040),
    127: check_test_set_frequency_exact_16(2, PWM_MODE_FAST, 16393),
    128: check_test_set_frequency_exact_16(2, PWM_MODE_FAST, 64516),
    129: check_test_set_frequency_exact_16(2, PWM_MODE_PHASE_CORRECT, 33333),
    130: check_test_set_frequency_exact_16(2, PWM_MODE_PHASE_CORRECT, 62500),
    131: check_test_set_frequency_exact_16(3, PWM_MODE_FAST, 16666),
    132: check_test_set_frequency_exact_16(3, PWM_MODE_FAST, 62500),
    133: check_test_set_frequency_exact_16(3, PWM_MODE_PHASE_CORRECT, 41666),
    134: check_test_set_frequency_exact_16(3, PWM_MODE_PHASE_CORRECT, 62500),
    135: check_test_set_frequency_exact_16(4, PWM_MODE_FAST, 62500),
    136: check_test_set_duty_cycle(0),
    137: check_test_set_duty_cycle(128),
    138: check_test_set_duty_cycle(255),
    139: check_test_set_duty_cycle(0),
    140: check_test_set_duty_cycle(128),
    141: check_test_set_duty_cycle(255),
    142: check_test_set_duty_cycle(0),
    143: check_test_set_duty_cycle(128),
    144: check_test_set_duty_cycle(255),
    145: check_test_set_duty_cycle(0),
    146: check_test_set_duty_cycle(128),
    147: check_test_set_duty_cycle(255),
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