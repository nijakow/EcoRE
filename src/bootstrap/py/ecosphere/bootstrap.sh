#!/bin/bash

SHELLPATH=$(dirname "$0")
export PYTHONPATH=$(dirname "$SHELLPATH")
python3 "${SHELLPATH}/bootstrapper.py" $1 $2
