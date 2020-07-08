#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"

#define UART_BAUD_RATE				115200

#define SYSTEM_PARTITION_RF_CAL_SZ              0x1000
#define SYSTEM_PARTITION_PHY_DATA_SZ            0x1000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ    0x3000

#define SYSTEM_SPI_SIZE				0x400000

#define SYSTEM_PARTITION_RF_CAL_ADDR            SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ - SYSTEM_PARTITION_RF_CAL_SZ
#define SYSTEM_PARTITION_PHY_DATA_ADDR          SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ - SYSTEM_PARTITION_PHY_DATA_SZ
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR  SYSTEM_SPI_SIZE - SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ

static os_timer_t start_timer;

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

void run_application(void* args)
{
	os_printf("Hello World\n");
}

void ICACHE_FLASH_ATTR user_init(void)
{
	uart_init(UART_BAUD_RATE, UART_BAUD_RATE);
	os_timer_setfn(&start_timer, (os_timer_func_t*)run_application, NULL);
	os_timer_arm(&start_timer, 1000, 1);
}

