from setup.utility import get_user_confirmation, safe_check_output, error, success
from setup.cpp_compilers import GCC, MicrosoftCompiler, Clang
from colorama import init as init_colorama
from setup.system_info import SystemInfo
from setup.installer import Installer
from setup.builder import Builder
from setup.cmake import CMake


class Setup:
    sysinfo: SystemInfo
    builder: Builder

    
    def __init__(self):
        self.sysinfo = SystemInfo.fetch()
        if self.sysinfo.os_bit_depth == 32:
            error("Sorry, but RubyEngine running only on 64-bit systems")
            self.setup_failed()

        print("------ RubyEngine Setup ------")
        print(f" OS: {self.sysinfo.os_name.capitalize()} {self.sysinfo.os_version}, x{self.sysinfo.os_bit_depth}")
        print(f" CPU Architecture: {self.sysinfo.architecture}")
        print(f" Logical processors: {self.sysinfo.threads_count}\n\n\n")


    def check_cmake(self) -> None:
        print("-- Checking CMake for availability... ", end="")
        if CMake.is_available():
            success("Available")
            return
        
        print("CMake does not found. To compile sources you need to install CMake.")
        if not get_user_confirmation("Would you like to install it now?"):
            self.setup_failed() 

        CMake.install(self.sysinfo.os_name)


    def check_cpp_compilers(self) -> None:
        print("-- Trying to found C++ compilers... ", end="", flush=True)

        compilers_dict = self.__get_cpp_compilers()
        if not len(compilers_dict):
            error(f"No one C++ compiler does not found.")
            if not get_user_confirmation("Would you like to install compiler(Clang) now?"):
                self.setup_failed()
            Installer.install_llvm(self.sysinfo.os_name)


        success(f"Found {len(compilers_dict)}:")
        for index, key in enumerate(compilers_dict.keys()):
            print(f" {index + 1}) {compilers_dict[key]}")

        c_compiler, cpp_compiler = self.__select_compiler(compilers_dict)
        print(f"-- C Compiler: {c_compiler}\n-- C++ Compiler: {cpp_compiler}")

        generator = self.__select_generator(cpp_compiler)
        print(f"-- CMake Generator: {generator}")


        self.builder = Builder(c_compiler, cpp_compiler, generator)


    def build_engine(self) -> None:
        if not self.builder.build():
            self.setup_failed()

    
    def setup_failed(self, several_new_lines: bool = True) -> None:
        if several_new_lines:
            print("\n\n", end="")
        error("-- Setup Failed")
        exit(1)


    def __get_cpp_compilers(self) -> dict:
        compilers =  {
            "cl": MicrosoftCompiler.get_if_available(),
            "clang++": Clang.get_if_available(),
            "g++": GCC.get_if_available()
        }

        return {key: value for key, value in compilers.items() if value != None}
    

    def __select_compiler(self, compilers_dict: dict) -> tuple[str]:
        c_compiler_map  = {
            "cl": "cl",
            "clang++": "clang",
            "g++": "gcc"
        }

        keys_list = list(compilers_dict.keys())
        if len(keys_list) == 1:           
            return (c_compiler_map[keys_list[0]], keys_list[0])

        selected_cpp_compiler = None
        for c in ["cl", "clang++", "g++"]:
            if c in compilers_dict:
                selected_cpp_compiler = c
                break
        
        print(f"More than one compiler found.\nCompiler by default: {compilers_dict[selected_cpp_compiler]}")
        print("Enter the number from the list above(or press Enter if you want to use default compiler): ", end="")

        while True:
            index = str(input())

            if index == "":
                if selected_cpp_compiler != None:
                    break
                error("Failed to detect default compiler. Please, specify it manually.")

            if index.isdigit() and (int(index) > 0 and int(index) <= len(keys_list)):
                selected_cpp_compiler = keys_list[int(index) - 1]
                break

            print("Incorrect index. Please, try again: ", end="")

        return (c_compiler_map[selected_cpp_compiler], selected_cpp_compiler)


    def __select_generator(self, selected_cpp_compiler: str) -> str:
        if safe_check_output("ninja --version") != None:
            return "Ninja"

        if selected_cpp_compiler == "cl":
            return CMake.get_newest_vs_generator()

        if selected_cpp_compiler in ["g++", "clang++"]:
            return "MinGW Makefiles" if self.sysinfo.os_name == "windows" else "Unix Makefiles"



if __name__ == "__main__" :
    setup = Setup()
    init_colorama()

    try:
        setup.check_cmake()
        setup.check_cpp_compilers()
        setup.build_engine()
    except KeyboardInterrupt:
        error("\n\n-- Setup was interrupted by a KeyboardInterrupt exception")
        setup.setup_failed(several_new_lines=False)
