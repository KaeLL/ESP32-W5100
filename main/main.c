
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_sntp.h"
#include "esp_log.h"

#include "w5100_main.h"
#include "mqtt_example.h"
#include "esp_http_client_example.h"

static const char *const TAG = "main";

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

	setenv( "TZ", CONFIG_TZ_ENV, 1 );
	tzset();

	sntp_setoperatingmode( SNTP_OPMODE_POLL );
	sntp_setservername( 0, "c.ntp.br" );
	sntp_init();

	int retry = 1;
	while ( sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET )
	{
		ESP_LOGI( TAG, "Waiting for system time to be set... (%d)", retry++ );
		vTaskDelay( pdMS_TO_TICKS( 2000 ) );
	}

	http_client_test();
	mqtt_example();
#ifdef CONFIG_TEST_DEINIT
	vTaskDelay( pdMS_TO_TICKS( 60000 ) );
	deinit();
#endif

	vTaskDelete( NULL );
}

void app_main( void )
{
	xTaskCreate( tasklol, "tasklol", 8192, NULL, 1, NULL );
}