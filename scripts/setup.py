from utility import get_user_confirmation, safe_check_output
from cpp_compilers import GCC, MicrosoftCompiler, Clang
from system_info import SystemInfo
from installer import Installer
from builder import Builder
from cmake import CMake


class Setup:
    sysinfo: SystemInfo
    builder: Builder

    
    def __init__(self):
        self.sysinfo = SystemInfo.fetch()
        if self.sysinfo.os_bit_depth == 32:
            print("Sorry, but RubyEngine running only on 64-bit systems")
            self.setup_failed()

        print("------ RubyEngine Setup ------")
        print(f" OS: {self.sysinfo.os_name} {self.sysinfo.os_version}")
        print(f" CPU Architecture: {self.sysinfo.arch}\n\n\n")


    def check_cmake(self) -> None:
        print("-- Checking CMake for availability... ", end="")
        if CMake.is_available():
            print("Available")
            return
        
        print("CMake does not found. To compile sources you need to install CMake.")
        if not get_user_confirmation("Would you like to install it now?"):
            self.setup_failed() 

        CMake.install(setup.sysinfo.os_name)


    def check_cpp_compilers(self) -> None:
        print("-- Trying to found C++ compilers... ", end="", flush=True)

        compilers = self.__get_cpp_compilers()
        if not len(compilers):
            print(f"No one C++ compiler does not found.")
            if not get_user_confirmation("Would you like to install compiler(Clang) now?"):
                self.setup_failed()
            Installer.install_llvm(self.sysinfo.os_name)


        print(f"Found {len(compilers)}:")
        for index, (key, value) in enumerate(compilers.items()):
            print(f" {index + 1}) {value}")

        c_compiler, cpp_compiler = self.__select_compiler(compilers)
        generator = self.__select_generator()
        print(f"-- CMake Generator: {generator}")

        self.builder = Builder(c_compiler, cpp_compiler, generator)


    def build_engine(self) -> None:
        if not self.builder.build():
            self.setup_failed()

    
    def setup_failed(self, several_new_lines: bool = True) -> None:
        if several_new_lines:
            print("\n\n", end="")
        print("-- Setup Failed")
        exit(1)


    def __get_cpp_compilers(self) -> dict:
        compilers =  {
            "cl": MicrosoftCompiler.get_if_available(),
            "clang++": Clang.get_if_available(),
            "g++": GCC.get_if_available()
        }

        return {key: value for key, value in compilers.items() if value != None}
    

    def __select_compiler(self, available_compilers: dict) -> tuple[str]:
        list_length = len(available_compilers)
        if list_length == 1:
            return next(iter(available_compilers.values()))

        compiler = available_compilers.get("cl") or available_compilers.get("clang++") or available_compilers.get("g++")

        print(f"More than one compiler found.\nCompiler by default: {compiler}")
        print("Enter the number from the list above(or press Enter if you want to use default compiler): ", end="")

        while True:
            index = str(input())

            if index == "":
                if compiler != None:
                    break
                print("Failed to detect default compiler. Please, specify it manually.")

            if index.isdigit() and (int(index) > 0 and int(index) <= list_length):
                compiler = available_compilers[int(index) - 1]
                break

            print("Incorrect input. Please, try again: ", end="")

        c_compiler = {
            "cl": "cl",
            "clang++": "clang",
            "g++": "gcc"
        }

        return (c_compiler[compiler], compiler)


    def __select_generator(self, selected_cpp_compiler: str) -> str:
        if safe_check_output("ninja --version") != None:
            return "Ninja"

        if selected_cpp_compiler == "cl":
            return CMake.get_newest_vs_generator()

        if selected_cpp_compiler in ["g++", "clang++"]:
            return "MinGW Makefiles" if self.sysinfo.os_name.lower() == "windows" else "Unix Makefiles"



if __name__ == "__main__" :
    setup = Setup()

    try:
        setup.check_cmake()
        setup.check_cpp_compilers()

        exit(0)
        setup.build_engine()
    except KeyboardInterrupt:
        print("\n\n-- Setup was interrupted by a KeyboardInterrupt exception")
        setup.setup_failed(several_new_lines=False)
