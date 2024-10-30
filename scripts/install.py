from utility import download_file



class Installer:
    @staticmethod
    def install_cmake():
        pass

    @staticmethod
    def install_llvm(os_name: str) -> bool:
        os_name = os_name.lower()
        assert(os_name == "windows" or os_name == "linux")

        target_url = "https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.0/"
        target_package = "LLVM-19.1.0-"
        match os_name:
            case "windows":
                target_package += f"win64.exe"
            case "linux":
                target_package += f"Linux-X64.tar.xz"    

        target_url += f"/{target_package}"
        print(f"Target URL for necessary package: {target_url}")
        print("Starting download...")
        
        if not download_file(target_url, target_package):
            return False
        

