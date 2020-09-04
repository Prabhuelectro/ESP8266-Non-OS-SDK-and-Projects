#!/bin/bash
# Copyright 2020 Prabhu Ramakrishnan
# E-Mail:prabhuelectro99@gmail.com
# Script will flash the ESP8266 binaries

BAUD=115200 #Change Baud Rate according to your need

F_MODE=dio #Change Flash Chip mode according to your need

F_FREQ=40m #Change Flash Chip Frequency according to your need


if [ "$1" == "default_bin" ]; then
	BIN_FOLDER=../bin
	chmod +rwx $3
	if [ $2 == 512KB ]
	then
		echo "Flash Size is: 512KB"
		esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0x7B000 ${BIN_FOLDER}/blank.bin 0x7C000 ${BIN_FOLDER}/esp_init_data_default_v08.bin 0x7E000 ${BIN_FOLDER}/blank.bin
	elif [ $2 == 1MB ]
	then
		echo "Flash Size is: 1MB"
		esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0xFB000 ${BIN_FOLDER}/blank.bin 0xFC000 ${BIN_FOLDER}/esp_init_data_default_v08.bin 0xFE000 ${BIN_FOLDER}/blank.bin
	elif [ $2 == 2MB ]
	then
		echo "Flash Size is: 3MB"
		esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0x1FB000 ${BIN_FOLDER}/blank.bin 0x1FC000 ${BIN_FOLDER}/esp_init_data_default_v08.bin 0x1FE000 ${BIN_FOLDER}/blank.bin
	else
		echo "Flash Size is: 4MB"
		esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0x3FB000 ${BIN_FOLDER}/blank.bin 0x3FC000 ${BIN_FOLDER}/esp_init_data_default_v08.bin 0x3FE000 ${BIN_FOLDER}/blank.bin
	fi

elif [ "$1" == "flash_bin" ]; then
	BIN_FOLDER=../bin
	chmod +rwx $3
	esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0x0000 ${BIN_FOLDER}/eagle.flash.bin 0x10000 ${BIN_FOLDER}/eagle.irom0text.bin

elif [ "$1" == "certs" ]; then
	BIN_FOLDER=bin
	chmod +rwx $3
	esptool.py --chip esp8266 --port $3 --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $2 0x77000 ${BIN_FOLDER}/esp_ca_cert.bin 0x78000 ${BIN_FOLDER}/esp_cert_private_key.bin
else
	printf "$0 [OPTIONS]\n"
	printf "OPTIONS:\n"
	printf "\n\tdefault_bin SIZE PORT\n"
	printf "\tflash_bin SIZE PORT\n"
	printf "\tcerts SIZE PORT\n"
	printf "\nAdjust the entries in script to change the Baud Rate and Flash Chip Frequency and Mode as your need\n"
fi
