from setup.utility import safe_check_output
from setup.installer import Installer
from colorama import Fore, Style
from subprocess import call
import shlex


class CMake:
    @staticmethod
    def is_available() -> bool:
        return safe_check_output("cmake --version") != None


    @staticmethod
    def install(os_name: str) -> bool:
        return Installer.install_cmake(os_name)


    @staticmethod
    def execute(args: str) -> bool:
        assert CMake.is_available()
        
        print(f"-- Running CMake command: {Fore.CYAN}cmake {args}{Style.RESET_ALL}")
        return call(shlex.split(f"cmake ")) == 0


    @staticmethod
    def get_newest_vs_generator() -> str | None:
        assert CMake.is_available()

        help_output = safe_check_output("cmake --help")
        help_output = help_output.split("\n")
        for line in help_output:
            if "Visual Studio" in line:
                return line[line.find("V"):line.find("=")].strip()        
            
        return None


    @staticmethod
    def get_default_generator() -> str | None:
        assert CMake.is_available()

        help_output = safe_check_output("cmake --help")
        help_output = help_output.split("\n")
        for line in help_output:
            if line.startswith("*"):
                return line[1:line.find("=")].strip()        
            
        return None
