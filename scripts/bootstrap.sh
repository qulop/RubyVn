#!/bin/bash

get_confirmation() {
    local prompt=$1
    echo -n "$prompt"
    while true; do
        read res
        res=$(echo "$res" | tr '[:upper:]' '[:lower:]')
        if [[ "$res" == "yes" || "$res" == "y" ]]; then
            echo true
            return 0
        elif [[ "$res" == "no" || "$res" == "n" ]]; then
            echo false
            return 0
        fi
        echo -n "Incorrect input. Please, try again: "
    done
}

enter_venv() {
    if [[ ! -d "venv" ]]; then
        python3 -m venv venv
    fi
    source ./venv/Scripts/activate
}

invoke_setup() {
    echo "Trying to create virtual environment..."
    if ! enter_venv; then
        if [[ "$(get_confirmation 'venv package not found. Would you like to install? [y/n]: ')" == "false" ]]; then
            echo "Failed to process bootstrap for setup script. Leaving..."
            exit 0
        fi
        python3 -m pip install --user virtualenv
        enter_venv
    fi
    echo "Installing the required libraries from requirements.txt..."
    ./venv/Scripts/pip3 install -r requirements.txt
    ./venv/Scripts/python setup.py
    deactivate
}

if command -v python3 &>/dev/null; then
    invoke_setup
    exit 0
fi

echo "Python not found."
if [[ "$(get_confirmation 'To continue setup you must install python. Would you like to continue? [y/n]: ')" == "false" ]]; then
    exit 0
fi

if command -v apt-get &>/dev/null; then
    sudo apt-get install -y python3 python3-pip
elif command -v yum &>/dev/null; then
    sudo yum install -y python3 python3-pip
elif command -v pacman &>/dev/null; then
    sudo pacman -S python python-pip
else
    echo "Unsupported package manager. Please install Python manually."
    exit 0
fi

invoke_setup
