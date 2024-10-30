from pathlib import Path
from tqdm import tqdm
import subprocess
import requests


def get_boolean_user_input(promptForInput) -> bool:
    promptForInput += " [Y/N]: "
    print(promptForInput, end="")
    while True:
        res = str(input()).strip().lower()
        match res:
            case "yes" | "y" | "true":
                return True
            case "no" | "n" | "false":
                return False
        
        print("Incorrect input. Please, try again: ", end="")


def download_file(url: str, filename: str) -> bool:
    response = requests.get(url, stream=True)
    total_size = int(response.headers.get("content-length", 0))
    block_size = 1024
    with tqdm(total=total_size, unit="ib", unit_scale=True) as progress_bar:
        with open(filename, "wb") as file:
            for chunk in response.iter_content(block_size):
                progress_bar.update(len(chunk))
                file.write(chunk)

    if total_size != 0 and progress_bar.n != total_size:
        return False
        
    return True


def safe_check_output(command: str) -> str | None:
    command = command.split(" ")
    try:
        output = subprocess.check_output(command)
        return output.decode()
    except subprocess.CalledProcessError or FileNotFoundError:
        return None
        

def find_recursive(directory: str, pattern: str) -> Path | None:
    for path in Path(directory).rglob(pattern):
        if str(path.name) == pattern:
            return path
    return None
