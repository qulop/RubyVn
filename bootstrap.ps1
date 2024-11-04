function Get-Confirmation() {
    param (
        [string]$prompt
    )

    Write-Output -NoNewline $prompt
    while ($true) {
        $res = Read-Host
        $res.ToLower()

        if ($res -eq "no" -or $res -eq "n") {
            return $false
        }
        elseif ($res -eq "yes" -or $res -eq "y") {
            return $true
        }

        Write-Output "Incorrect input. Please, try again: "
    }
}

function Enter-Venv() {
    if (-not (Test-Path -Path "venv" -PathType Container)) {
        python3 -m venv venv
    }

    .\venv\Scripts\Activate.ps1
}

function Invoke-Setup() {
    Write-Output "Trying to create virtual enviroment..."

    Set-Location -Path "scripts/"
    try {
        Enter-Venv
    }
    catch  {
        if (-not (Get-Confirmation "venv package does not found. Would you like to install? [y/n]")) {
            Write-Output "Failed to process bootstrap for setup script. Leaving..."
            exit 0
        }

        python3 -m pip install --user virtualenv
        Enter-Venv
    }

    Write-Output "Installing the required libraries from requirements.txt..."
    .\venv\Scripts\pip3.exe install -r requirements.txt

    .\venv\Scripts\python.exe main.py
    deactivate
    Set-Location -Path "../"
}



$is_python = Get-Command python3 -ErrorAction SilentlyContinue
if ($is_python) {
    Invoke-Setup
    exit 0
}

Write-Output "Python does not found." -NoNewline
if (-not (Get-Confirmation "To continue setup you must to install python. Would you like to continue? [y/n]")) {
    exit 0
}

# TODO: Python installing
