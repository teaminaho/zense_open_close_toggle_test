#!/bin/bash
timestamp=$(date "+%Y.%m.%d-%H.%M.%S")
./openCloseToggle 2>&1 | tee -a "logs/${timestamp}.txt"