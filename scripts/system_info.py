import platform
from dataclasses import dataclass


@dataclass
class SystemInfo:
    os_name: str
    os_version: str
    os_bit_depth: int
    arch: str

    def __init__(self, os_name: str, os_version: str, os_bit_depth: int, arch: str):
        self.os_name = os_name
        self.os_version = os_version
        self.os_bit_depth = os_bit_depth
        self.arch = arch

    @staticmethod
    def fetch():
        os_name = platform.system()
        os_version = platform.version()
        arch = platform.machine()
        os_bit_depth = int(platform.architecture()[0].replace("bit", "").strip())

        match arch:
            case "x86_64" | "AMD64":
                arch = "x86_64"
            case "i386" | "i686":
                arch = "x86"
            case arch.startswith("arm"):
                arch = "arm"
            case arch.startswith("riscv"):
                arch = "RISC-V"
            case _:
                arch = "unknown"

        return SystemInfo(os_name, os_version, os_bit_depth, arch)

