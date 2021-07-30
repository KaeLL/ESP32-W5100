
#include "sdkconfig.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "w5100_main.h"

void tasklol( void *p )
{
	ESP_ERROR_CHECK( spi_bus_initialize(
		SPI3_HOST,
		&( spi_bus_config_t ) {
			.miso_io_num = GPIO_NUM_19,
			.mosi_io_num = GPIO_NUM_23,
			.sclk_io_num = GPIO_NUM_18,
			.max_transfer_sz = 4,
			.quadwp_io_num = -1,
			.quadhd_io_num = -1 },
		1 ) );

	// Initialize TCP/IP network interface (should be called only once in application)
	ESP_ERROR_CHECK( esp_netif_init() );
	// Create default event loop that running in background
	ESP_ERROR_CHECK( esp_event_loop_create_default() );

	w5100_start();

	vTaskDelete( NULL );
}

void app_main( void )
{
	xTaskCreate( tasklol, "tasklol", 8192, NULL, 1, NULL );
}