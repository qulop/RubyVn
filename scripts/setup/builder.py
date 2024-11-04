from setup.utility import find_recursive, success
from setup.cmake import CMake
from setup.system_info import SystemInfo
from pathlib import Path
import shutil
import os


class Builder:
    ROOT_DIR: Path = Path(os.path.dirname(__file__)).parent.parent
    SCRIPTS_DIR: Path = Path(os.path.dirname(__file__)).parent

    __generator: str = None
    __c_compiler: str = None
    __cpp_compiler: str = None


    def __init__(self, c_compiler: str, cpp_compiler: str, generator: str):
        self.__c_compiler = c_compiler
        self.__cpp_compiler = cpp_compiler
        self.__generator = generator


    def build(self) -> bool:
        assert self.__generator != None

        success(f"ROOT: {self.ROOT_DIR}")
        success(f"SCRIPTS: {self.SCRIPTS_DIR}")

        os.chdir(str(self.ROOT_DIR))
        if os.path.isdir("build"):
            shutil.rmtree("build")
        os.mkdir("build")
        os.chdir("build")

        if not self.__run_cmake():
            return False

        location = str(find_recursive(os.getcwd(), "RubyEngine.exe"))
        success(f"\n\n-- Build complete\n-- Location: {location}\n\n")

        os.chdir(str(self.SCRIPTS_DIR))
        return True
    

    def __run_cmake(self) -> bool:
        success(f"DIR {os.getcwd()}")
        config_args = f"-G '{self.__generator}' -A x64 -DCMAKE_C_COMPILER={self.__c_compiler} -DCMAKE_CXX_COMPILER={self.__cpp_compiler} .."
        compile_args = "--build . --target RubyEngine --config Release "

        if "visual studio" in self.__generator.lower():
            compile_args += f"/MP"
        else:
            compile_args += f" -j {SystemInfo.fetch().threads_count}"

        return (CMake.execute(config_args)) and (CMake.execute(compile_args))