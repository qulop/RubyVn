from utility import get_boolean_user_input, safe_check_output, find_recursive
from system_info import SystemInfo
from install import Installer
from subprocess import call
from pathlib import Path
import os


class Setup:
    sysinfo: SystemInfo
    
    def __init__(self):
        self.sysinfo = SystemInfo.fetch()
        if self.sysinfo.os_bit_depth == 32:
            print("Sorry, but RubyEngine running only on 64-bit systems")
            exit(1)

        print("------ RubyEngine Setup ------")
        print(f" OS: {self.sysinfo.os_name} {self.sysinfo.os_version}")
        print(f" CPU Architecture: {self.sysinfo.arch}\n\n\n")


    def is_cmake_installed(self) -> bool:
        return safe_check_output("cmake --version") != None
        
    def get_cpp_compilers(self) -> list[str]:
        compilers = [safe_check_output("clang++ --version"), self.__check_msvc(), safe_check_output("g++ --version")]
        compilers = list(filter(lambda compiler: compiler != None, compilers))

        return [c.split("\n")[0] for c in compilers]

    def __check_msvc(self) -> str | None:
        return None


class Builder:
    ROOT_DIR = Path(os.path.dirname(__file__)).parent
    SCRIPTS_DIR = Path(os.getcwd())


    def build_engine(self) -> bool:
        os.chdir(str(self.ROOT_DIR))
        if not os.path.isdir("build"):
            os.mkdir("build")
        os.chdir("build")

        if not self.__run_cmake():
            return False

        location = str(find_recursive(os.getcwd(), "RubyEngine.exe"))
        print(f"\n\n-- Build complete\n-- Location: {location}\n\n")

        os.chdir(str(self.SCRIPTS_DIR))
        return True
    
    def __run_cmake(self) -> bool:
        commands = ("cmake ..", "cmake --build . --target RubyEngine")

        return (call(commands[0].split()) == 0) and (call(commands[1].split()) == 0)



if __name__ == "__main__" :
    setup = Setup()
    builder = Builder()


    print("-- Checking CMake for availability... ", end="")
    if setup.is_cmake_installed():
        print("Available")
    else:
        print("CMake does not found. To compile sources you need to install CMake.")
        if get_boolean_user_input("Would you like to install it now?"):
            Installer.install_cmake(setup.sysinfo.os_name)


    print("-- Trying to found any C++ compiler... ", end="")
    compilers = setup.get_cpp_compilers()
    if len(compilers):
        print("Available compilers:")
        for c in compilers:
            print(f" >> {c}")
    else:
        print(f"No one C++ compiler does not found.")
        if get_boolean_user_input("Would you like to install compiler(Clang) now?"):
            Installer.install_llvm(setup.sysinfo.os_name)

    if not builder.build_engine():
        print("-- Failed to asseble an engine")
