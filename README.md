# ESP8266 Non-OS SDK and Projects

## blink_leds 
## Blink NodeMCU Board LED (GPIO2) at 500 Milli Seconds Delay

## dht_sensor
           ## DHT22 Sensor pin GPIO2 and print Temperature and Humidity in UART(Serial)
		
## hello_world
		## Print Hello World in UART(Serial) at 1 Second Delay

## http_server
		## HTTP Server with Station Mode ESP-LED 

## print_mem_info
		## Print ESP8266 Memory information

## sdk_version
		## Print ESP8266 Non-OS SDK version

 
# How to build ESP8266 Non-OS SDK projects ?

	## Enter into the project directory 

	```$ cd blink_leds/```

	## Change the script to executable

	```$ chmod +x gen_misc.sh```

	##  Run the gen_misc.sh script

	```$ ./gen_misc.sh```

	## Give the specific options in the STEPS and flash the eagle binary to ESP8266

	```$ ./../tools/flash_firmware.sh```

