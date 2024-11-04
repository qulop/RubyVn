import platform
from psutil import cpu_count
from dataclasses import dataclass


@dataclass
class SystemInfo:
    os_name: str
    os_version: str
    os_bit_depth: int
    architecture: str
    threads_count: int

    def __init__(self, os_name: str, os_version: str, os_bit_depth: int, arch: str, th_count: int):
        self.os_name = os_name
        self.os_version = os_version
        self.os_bit_depth = os_bit_depth
        self.architecture = arch
        self.threads_count = th_count

    @staticmethod
    def fetch() -> 'SystemInfo':
        os_name = platform.system().lower()
        os_version = platform.version()
        architecture = platform.machine()
        os_bit_depth = int(platform.architecture()[0].replace("bit", "").strip())
        threads_count = cpu_count(logical=True)

        match architecture:
            case "x86_64" | "AMD64":
                architecture = f"x86_64({architecture})"
            case "i386" | "i686":
                architecture = f"x86"
            case architecture.startswith("arm"):
                architecture = "arm"
            case architecture.startswith("riscv"):
                architecture = "RISC-V"
            case _:
                architecture = "unknown"

        return SystemInfo(os_name, os_version, os_bit_depth, architecture, threads_count)
    
