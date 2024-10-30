from system_info import SystemInfo
from utility import get_boolean_user_input, safe_check_output
from install import Installer



class Setup:
    sysinfo: SystemInfo
    __is_setup_success: bool = False

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
        
    def get_cpp_compiler(self) -> list[str]:
        compilers = [safe_check_output("clang++ --version"), self.__check_msvc(), safe_check_output("g++ --version")]
        compilers = list(filter(lambda compiler: compiler != None, compilers))

        return [c.split("\n")[0] for c in compilers]

    def __check_msvc(self) -> str | None:
        return None


if __name__ == "__main__" :
    setup = Setup()


    print("-- Checking CMake for availability... ", end="")
    if setup.is_cmake_installed():
        print("Available")
    else:
        print("CMake does not found. To compile sources you need to install CMake.")
        if get_boolean_user_input("Would you like to install it now?"):
            Installer.install_cmake()


    print("-- Trying to found any C++ compiler... ", end="")
    compilers = setup.get_cpp_compiler()
    if len(compilers):
        print("Available compilers:")
        for c in compilers:
            print(f" >> {c}")
    else:
        print(f"C++ compiler does not found.")
        if get_boolean_user_input("Would you like to install compiler now(Clang)?"):
            Installer.install_llvm(setup.sysinfo.os_name)
