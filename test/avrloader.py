
from dataclasses import dataclass
import subprocess

LOADER_BINARY = "avrdude"
DEFAULT_PROGRAMMER = "arduino"
DEFAULT_BAUDS = 19200

class LoadError(Exception):
    pass

@dataclass
class Loader:
    device: str
    programmer: str = DEFAULT_PROGRAMMER
    bauds: int = DEFAULT_BAUDS

    def load(self, program: str, partno: str, override_invalid_signature_check: bool = True, verify: bool = False) -> None:
        cmd = [LOADER_BINARY, "-c", self.programmer, "-p", partno, "-P", self.device, "-b", str(self.bauds), "-U", f"flash:w:{program}"]
        if override_invalid_signature_check:
            cmd.append("-F")
        if not verify:
            cmd.append("-V")
        result = subprocess.run(cmd, capture_output=True)
        print(result.stdout.decode("ascii"))
        print(result.stderr.decode("ascii"))
        if result.returncode:
            raise LoadError(result.stderr.decode("ascii"))
