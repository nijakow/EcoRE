#!/bin/bash

# Check if we are on Debian or Fedora, and call either deps.debian.sh or deps.fedora.sh

if [ -f /etc/debian_version ]; then
    echo "Debian detected, calling deps.debian.sh"
    ./scripts/deps.debian.sh
elif [ -f /etc/fedora-release ]; then
    echo "Fedora detected, calling deps.fedora.sh"
    ./scripts/deps.fedora.sh
else
    echo "Unknown distribution, please install the dependencies manually."
fi
