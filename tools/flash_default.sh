F_SIZE=1MB

PORT=/dev/ttyUSB0

BAUD=115200

F_MODE=dio

F_FREQ=40m

BIN_FOLDER=../bin

chmod +rwx $PORT

esptool.py --chip esp8266 --port $PORT --baud $BAUD --before default_reset --after hard_reset write_flash --flash_mode $F_MODE --flash_freq $F_FREQ --flash_size $F_SIZE 0xFB000 ${BIN_FOLDER}/blank.bin 0xFC000 ${BIN_FOLDER}/esp_init_data_default_v08.bin 0xFE000 ${BIN_FOLDER}/blank.bin
