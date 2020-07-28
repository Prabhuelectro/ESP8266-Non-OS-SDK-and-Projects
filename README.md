# ESP8266 Non-OS SDK and Projects

## blink_leds</br> 
## Blink NodeMCU Board LED (GPIO2) at 500 Milli Seconds Delay

## dht_sensor</br>
## DHT22 Sensor pin GPIO2 and print Temperature and Humidity in UART(Serial)
		
## hello_world</br>
## Print Hello World in UART(Serial) at 1 Second Delay

## http_server</br>
## HTTP Server with Station Mode ESP-LED 

## print_mem_info</br>
## Print ESP8266 Memory information

## sdk_version</br>
## Print ESP8266 Non-OS SDK version

 
# How to build ESP8266 Non-OS SDK projects ?</br>

	## Enter into the project directory</br>

	```$ cd blink_leds/```

	## Change the script to executable</br>

	```$ chmod +x gen_misc.sh```

	##  Run the gen_misc.sh script</br>

	```$ ./gen_misc.sh```

	## Give the specific options in the STEPS and flash the eagle binary to ESP8266</br>

	```$ ./../tools/flash_firmware.sh```

