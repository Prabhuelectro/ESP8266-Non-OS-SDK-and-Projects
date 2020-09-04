#include "dht22.h"

#define MIN_INTERVAL 2000
#define NUM_VALID_INPUT_PINS 8
const uint32_t pin_mux[NUM_VALID_INPUT_PINS] = {PERIPHS_IO_MUX_GPIO0_U,PERIPHS_IO_MUX_GPIO2_U,PERIPHS_IO_MUX_GPIO4_U,PERIPHS_IO_MUX_GPIO5_U,PERIPHS_IO_MUX_MTDI_U,PERIPHS_IO_MUX_MTCK_U,PERIPHS_IO_MUX_MTMS_U,PERIPHS_IO_MUX_MTDO_U}; 
const uint8_t pin_num[NUM_VALID_INPUT_PINS] = {0,2,4,5,12,13,14,15};
const uint8_t pin_func[NUM_VALID_INPUT_PINS] = {FUNC_GPIO0, FUNC_GPIO2,FUNC_GPIO4,FUNC_GPIO5,FUNC_GPIO12,FUNC_GPIO13,FUNC_GPIO14,FUNC_GPIO15};


float ICACHE_FLASH_ATTR convertCtoF(float c) {
	return c * 1.8 + 32;
}

float ICACHE_FLASH_ATTR convertFtoC(float f) {
	return (f - 32) * 0.55555;
}

static int ICACHE_FLASH_ATTR get_index(int pin)
{
	int i = 0;
	for(i = 0; i < NUM_VALID_INPUT_PINS; i++)
	{
		if(pin==pin_num[i])
		{
			return i;
		}
	}
	return -1;
}

void ICACHE_FLASH_ATTR DHT_begin(void) 
{
	LOG_DEBUG("DHT BEGIN");

	// set up the pins!
	int index = get_index(_pin);

	gpio_init();
	PIN_FUNC_SELECT(pin_mux[index],pin_func[index]); 
	//GPIO_OUTPUT_SET(_pin,1);
	GPIO_DIS_OUTPUT(_pin);
	PIN_PULLUP_EN(pin_mux[index]);

	// Using this value makes sure that millis() - lastreadtime will be >= MIN_INTERVAL right away. Note that this assignment wraps around, but so will the subtraction.
	_lastreadtime = -MIN_INTERVAL;
}

static bool read() {

	int index = get_index(_pin);

	// Check if sensor was read less than two seconds ago and return early to use last reading. 
	//This is todo.  The force argument overrides this logic.  We can implement in ESP8266 with a timer.
	uint32 current =  system_get_time();
	if ( current - _lastreadtime < 2000000 )
	{	
		LOG_DEBUG_ARGS("2 seconds is required between polls,  Its only been %d (ms)",(current-_lastreadtime) / 1000);
		return _lastresult;
	}
	_lastreadtime=current;


	// Reset 40 bits of received data to zero.
	data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	// Send start signal.  See DHT datasheet for full signal diagram:
	//   http://www.adafruit.com/datasheets/Digital%20humidity%20and%20temperature%20sensor%20AM2302.pdf

	// Go into high impedence state to let pull-up raise data line level and
	// start the reading process.
	GPIO_OUTPUT_SET(_pin,1);
	os_delay_us(250*1000);


	// First set data line low for 10 milliseconds.
	GPIO_OUTPUT_SET(_pin,0);
	os_delay_us(10*1000);

	uint32_t cycles[80];
	{
		// Turn off interrupts temporarily because the next sections are timing critical
		// and we don't want any interruptions.
		//TODO: We've disabled GPIO interrupts, but we haven't done anything about timers
		ETS_GPIO_INTR_DISABLE();

		// End the start signal by setting data line high for 40 microseconds.
		GPIO_OUTPUT_SET(_pin,1);
		os_delay_us(40);

		// Now start reading the data line to get the value from the DHT sensor.
		GPIO_DIS_OUTPUT(_pin);
		PIN_PULLUP_EN(pin_mux[index]);

		//os_delay_us(100);  // Delay a bit to let sensor pull data line low.

		// First expect a low signal for ~80 microseconds followed by a high signal
		// for ~80 microseconds again.
		if (expectPulse(0) == 0) 
		{
			LOG_DEBUG("Timeout waiting for start signal low pulse.");
			_lastresult = false;
			return _lastresult;
		}
		if (expectPulse(1) == 0) 
		{
			LOG_DEBUG("Timeout waiting for start signal high pulse.");
			_lastresult = false;
			return _lastresult;
		}

		// Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
		// microsecond low pulse followed by a variable length high pulse.  If the
		// high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
		// then it's a 1.  We measure the cycle count of the initial 50us low pulse
		// and use that to compare to the cycle count of the high pulse to determine
		// if the bit is a 0 (high state cycle count < low state cycle count), or a
		// 1 (high state cycle count > low state cycle count). Note that for speed all
		// the pulses are read into a array and then examined in a later step.
		int i = 0;
		for (i=0; i<80; i+=2) 
		{
			cycles[i]   = expectPulse(0);
			cycles[i+1] = expectPulse(1);
		}
	} // Timing critical code is now complete.
	ETS_GPIO_INTR_ENABLE();

	// Inspect pulses and determine which ones are 0 (high state cycle count < low
	// state cycle count), or 1 (high state cycle count > low state cycle count).
	int i = 0;
	for (i=0; i<40; ++i) 
	{
		uint32_t lowCycles  = cycles[2*i];
		uint32_t highCycles = cycles[2*i+1];
		if ((lowCycles == 0) || (highCycles == 0)) 
		{
			LOG_DEBUG("Timeout waiting for pulse.");
			_lastresult = false;
			return _lastresult;
		}
		data[i/8] <<= 1;
		// Now compare the low and high cycle times to see if the bit is a 0 or 1.
		if (highCycles > lowCycles) 
		{
			// High cycles are greater than 50us low cycle count, must be a 1.
			data[i/8] |= 1;
		}
		// Else high cycles are less than (or equal to, a weird case) the 50us low
		// cycle count so this must be a zero.  Nothing needs to be changed in the
		// stored data.
	}

	LOG_DEBUG_ARGS("Received: %d,%d,%d,%d,%d ?= %d",data[0],data[1],data[2],data[3],data[4],(data[0] + data[1] + data[2] + data[3]) & 0xFF );



	// Check we read 40 bits and that the checksum matches.
	if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) 
	{
		_lastresult = true;
	
		float f = data[0];
		f *= 256;
		f += data[1];
		f *= 0.1;

		f = data[2] & 0x7F;
	      	f *= 256;
	      	f += data[3];
	      	f *= 0.1;
	      	if (data[2] & 0x80) {
        		f *= -1;
      		}
		return _lastresult;
	}
	else 
	{
		LOG_DEBUG("Checksum failure!");
		_lastresult = false;
		return _lastresult;
	}
}

// Expect the signal line to be at the specified level for a period of time and
// return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond
// ellapses without the level changing then the call fails with a 0 response.
// This is adapted from Arduino's pulseInLong function (which is only available
// in the very latest IDE versions):
//   https://github.com/arduino/Arduino/blob/master/hardware/arduino/avr/cores/arduino/wiring_pulse.c
uint32_t expectPulse(bool level) 
{
	uint32_t count = 0;
	while (GPIO_INPUT_GET(_pin) == level) 
	{
		if (count++ >= _maxcycles) 
		{
			LOG_DEBUG_ARGS("Max cycles reached: %d",count);
			return 0; // Exceeded timeout, fail.
      		}
    	}
	return count;
}

void DHT_init(uint8_t pin, uint8_t type)
//void DHT_init(uint8_t pin, uint8_t type, uint8_t count)
{
	LOG_DEBUG("DHT_INIT");
	_pin = pin;
	_type = type;

	//_maxcycles gives thenumber of cycles in a millisecond. this is used as a timeout in other functions.

	//bit11~bit0 are decimal part
	uint32_t cal = system_rtc_clock_cali_proc();
	uint32_t int_part = ((cal*1000) >> 12) / 1000;// + ( (float)(((cal*1000)>>12)%1000 ) / 1000.0f);
	uint32_t dec_part = ((cal * 1000) >> 12) % 1000; 

	float result = int_part + (float)dec_part/1000.0f;

	_maxcycles = 1000.0f / result;
	LOG_DEBUG_ARGS("max cycles: %d",(int)_maxcycles);
}


float readHumidity()
{
	float f = -1;
	if (read()) 
	{
		switch (_type) 
		{
			case DHT11:
				f = data[0];
				break;
			case DHT22:
			case DHT21:
				f = data[0];
				f *= 256;
				f += data[1];
				f *= 0.1;
				break;
		}
	}
	return f;
}

//boolean S == Scale.  True == Fahrenheit; False == Celcius
float readTemperature(bool S) {
	float f = -1;

	if (read()) 
	{
		switch (_type) 
		{
			case DHT11:
				f = data[2];
				if(S) 
				{
					f = convertCtoF(f);
				}
			break;
			case DHT22:
			case DHT21:
				f = data[2] & 0x7F;
				f *= 256;
				f += data[3];
				f *= 0.1;
				if (data[2] & 0x80) 
				{
					f *= -1;
				}
				if(S) 
				{
					f = convertCtoF(f);
				}
				break;
		}
	}
	return f;
}

