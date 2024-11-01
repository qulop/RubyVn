from subprocess import run, CalledProcessError
from utility import download_file
from pathlib import Path


class Installer:
    @staticmethod
    def __run_installer(file_path: Path) -> bool:
        file = str(file_path)
        try:
            run([file], check=True)
        except CalledProcessError:
            return False
        except FileNotFoundError:
            print(f"FileNotFoundError: {file}")
            return False
        
        return True
    
    @staticmethod
    def __install_package(os_name: str, target_url: str, win32_package: str, linux_package: str) -> bool:
        os_name = os_name.lower()
        assert(os_name == "windows" or os_name == "linux")

        target_package = win32_package if os_name == "windows" else linux_package
        target_url += f"{target_package}"
        print(f"Target URL for necessary package: {target_url}")
        print("Starting download...")
        
        file_path = Path.home() / "Downloads" / target_package
        if not download_file(target_url, str(file_path)):
            return False
        
        return Installer.__run_installer(file_path)


    @staticmethod
    def install_cmake(os_name: str) -> bool:
        return Installer.__install_package(os_name=os_name, 
                                        target_url="https://github.com/Kitware/CMake/releases/download/v3.31.0-rc3/",
                                        win32_package="cmake-3.31.0-rc3-windows-x86_64.msi", 
                                        linux_package="cmake-3.31.0-rc3.tar.gz")
    
    @staticmethod
    def install_llvm(os_name: str) -> bool:
        return Installer.__install_package(os_name=os_name, 
                                    target_url="https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.0/",
                                    win32_package="LLVM-19.1.0-win64.exe",
                                    linux_package="LLVM-19.1.0-Linux-X64.tar.xz")
        

