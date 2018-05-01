#!/bin/bash

## RAW_LoRa data logger
## Sergio Rivera Lavado

stty -F /dev/ttyACM0 115200
while true; do
  read -r f < /dev/ttyACM0
  u=$(date +%D/%H%M%S%N)
  if [ $(echo $f | grep -o , | wc -l) -ge 5 ];
    then echo $u','$f >> RAW_lora_adquired;
  else sleep 1
  fi
done
