
import os
import subprocess

LOADER_BINARY = "avrdude"
ARDUINO_PROGRAMMER = "arduino"

class LoadError(Exception):
    pass

def load(program: str, device: str, programmer: str, partno: str, bauds: int, override_invalid_signature_check: bool = True, verify: bool = False) -> None:
    cmd = [LOADER_BINARY, "-c", programmer, "-p", partno, "-P", device, "-b", str(bauds), "-U", f"flash:w:{program}"]
    if override_invalid_signature_check:
        cmd.append("-F")
    if not verify:
        cmd.append("-V")
    result = subprocess.run(cmd, capture_output=True)
    print(result.stdout.decode("ascii"))
    print(result.stderr.decode("ascii"))
    if result.returncode:
        raise LoadError(result.stderr.encode("ascii"))
