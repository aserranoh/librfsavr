
import os
import sys

PASS_RETURN_VALUE = 0
FAIL_RETURN_VALUE = 1
SKIP_RETURN_VALUE = 77

def get_environment_variable(variable: str) -> str:
    return os.environ[variable]

def pass_() -> None:
    sys.exit(PASS_RETURN_VALUE)

def skip() -> None:
    sys.exit(SKIP_RETURN_VALUE)

def fail() -> None:
    sys.exit(FAIL_RETURN_VALUE)