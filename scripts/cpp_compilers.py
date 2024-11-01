from utility import safe_check_output, find_recursive
from system_info import SystemInfo
from subprocess import PIPE, run
from abc import ABC
from re import search
import os


def __extract_compiler_version(raw_version: str, pattern: str = r"[\d]+.[\d]+.[\d]+") -> str | None:
    if not raw_version:
        return None

    version = search(r"[\d]+.[\d]+.[\d]+", raw_version)
    return version.group() if version else None


def __extract_major_compiler_version(raw_version: str, pattern: str = r"[\d]+.[\d]+.[\d]+") -> str | None:
    version = __extract_compiler_version(raw_version, pattern)

    return int(version.split('.')[0]) if version else None 




class ICompiler(ABC):
    @staticmethod
    def get_if_available() -> bool:
        pass

    @staticmethod
    def get_major_version() -> int | None:
        return __extract_major_compiler_version(ICompiler.get_if_available())

    @staticmethod
    def get_version() -> str | None:
        return __extract_compiler_version(ICompiler.get_if_available())



class GCC(ICompiler):
    @staticmethod
    def get_if_available() -> str | None:
        return safe_check_output("g++ --version")


class Clang(ICompiler):
    @staticmethod
    def get_if_available() -> str | None:
        return safe_check_output("clang++ --version")



class MicrosoftCompiler(ICompiler):
    @staticmethod
    def get_if_available() -> str | None:
        return MicrosoftCompiler.__check_availability()
    

    @staticmethod
    def __check_availability() -> str | None:
        if SystemInfo.fetch().os_name.lower() != "windows":
            return None

        vswhere_path = MicrosoftCompiler.__find_vswhere()
        if not vswhere_path:
            return None

        vswhere_command = f"{vswhere_path} -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath"
        installation_path = safe_check_output(vswhere_command)
        if not installation_path:
            return None
        
        cl_path = find_recursive(installation_path.strip(), "cl.exe")
        if not cl_path:
            return None

        version = run([cl_path], stdout=PIPE, stderr=PIPE, text=True)
        return version.stderr.split("\n")[0]
        

    @staticmethod
    def __find_vswhere() -> str | None:
        path = "C:\\Program Files (x86)\\Microsoft Visual Studio\\Installer\\vswhere.exe"
        if os.path.isfile(path):
            return path
        
        path = find_recursive("C\\", "vswhere.exe")
        return str(path) if path else None