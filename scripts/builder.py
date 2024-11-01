from utility import find_recursive
from pathlib import Path
from cmake import CMake
import os


class Builder:
    ROOT_DIR: Path = Path(os.path.dirname(__file__)).parent
    SCRIPTS_DIR: Path = Path(os.getcwd())

    __generator: str = None
    __c_compiler: str = None
    __cpp_compiler: str = None


    def __init__(self, c_compiler: str, cpp_compiler: str, generator: str):
        self.__c_compiler = c_compiler
        self.__cpp_compiler = cpp_compiler
        self.__generator = generator


    def build(self) -> bool:
        assert self.__generator != None

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
        config_args = f"-G \"{self.__generator}\" -DCMAKE_C_COMPILER={self.__c_compiler} -DCMAKE_CXX_COMPILER={self.__cpp_compiler} .."
        compile_args = "--build . --target RubyEngine"
        if self.__generator.lower() == "ninja":
            compile_args += " -j 10"

        return (CMake.execute(config_args)) and (CMake.execute(compile_args))