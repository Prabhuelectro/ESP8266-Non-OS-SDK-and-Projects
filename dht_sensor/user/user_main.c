#include <ets_sys.h>
#include <osapi.h>
#include <gpio.h>
#include <os_type.h>
#include <mem.h>
#include "user_interface.h"
#include "dht22.h"

volatile os_timer_t read_humidity_timer;
void read_humidity_timerfunc(void);

#define UART_BAUD_RATE				115200

#define SYSTEM_PARTITION_RF_CAL_SZ              0x1000
#define SYSTEM_PARTITION_PHY_DATA_SZ            0x1000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ    0x3000

#define SYSTEM_SPI_SIZE				0x400000

#define SYSTEM_PARTITION_RF_CAL_ADDR            SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ - SYSTEM_PARTITION_RF_CAL_SZ
#define SYSTEM_PARTITION_PHY_DATA_ADDR          SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR  SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ

static const partition_item_t part_table[] =
{
	{ SYSTEM_PARTITION_RF_CAL,              SYSTEM_PARTITION_RF_CAL_ADDR,           SYSTEM_PARTITION_RF_CAL_SZ              },
	{ SYSTEM_PARTITION_PHY_DATA,            SYSTEM_PARTITION_PHY_DATA_ADDR,         SYSTEM_PARTITION_PHY_DATA_SZ            },
	{ SYSTEM_PARTITION_SYSTEM_PARAMETER,    SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR, SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ    }
};

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	system_partition_table_regist(part_table, 3, SPI_FLASH_SIZE_MAP);
}

void user_init(void)
{
        uart_div_modify(0, UART_CLK_FREQ /UART_BAUD_RATE );
        os_delay_us(10);

        //Setup a timer to read DHT every 3 seconds, on repeat.
        os_timer_setfn(&read_humidity_timer, (os_timer_func_t *)read_humidity_timerfunc, NULL);
        os_timer_arm(&read_humidity_timer, 3000, 1);

        //Declare we are using a DHT22 on Pin 14
        DHT_init(2,DHT22);
        DHT_begin();
}


void read_humidity_timerfunc(void)
{
        float t = readTemperature(false);
        float h = readHumidity();

        os_printf("Temperature (C): %d\r\n", (int)t);
        os_printf("Humidity (C): %d\r\n", (int)h);
}

