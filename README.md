# ESP8266 Non-OS SDK and Projects

## blink_leds
- Blink NodeMCU Board LED (GPIO2) at 500 Milli Seconds Delay

## dht_sensor
- DHT22 Sensor pin GPIO2 and print Temperature and Humidity in UART(Serial)
		
## hello_world
- Print Hello World in UART(Serial) at 1 Second Delay

## http_server
- HTTP Server with Station Mode ESP-LED 

## print_mem_info
- Print ESP8266 Memory information

## sdk_version
- Print ESP8266 Non-OS SDK version


# How to build ESP8266 Non-OS SDK projects ?

- Clone the ESPRESSIF ESP8266 NON OS SDK
```sh
$ git clone https://github.com/Prabhuelectro/ESP8266-Non-OS-SDK-and-Projects.git
$ cd ESP8266-Non-OS-SDK-and-Projects/
```
- Download and Install ESP8266 toolchain for Linux 64 Bit
```sh
$ wget https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
$ tar xvf xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz
$ cd xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0/bin
$ pwd
```
- Copy the toolchain bin path and set in to the $PATH variable or set it permanently
```sh
$ export PATH="$PATH:$PWD"
```
- setting permanently by bellow
```sh
$ echo "PATH="$PATH:/home/prabhu/workspace/ESP8266/xtensa-lx106-elf/bin"" >> ~/.bashrc
```

- Now start to write ESP8266 Non OS SDK Programs

## Enter into the project directory
```sh
$ cd blink_leds/
```

## Change the script to executable
```sh
$ chmod +x gen_misc.sh
```

## Run the gen_misc.sh script
## Give the specific options in the STEPS to build binary's for ESP8266
```sh
$ ./gen_misc.sh
```

## How to flash Build files
- At first time flashing ESP8266 by run the following script 
- Adjust the script as your need the flash size and flash freqency and flash mode and PORT 

```sh
$ ./../tools/flash_default.sh
```

- Then flash the build files by
- Adjust the script as your need the flash size and flash freqency and flash mode and PORT 
```sh
$ ./../tools/flash_firmware.sh
```
