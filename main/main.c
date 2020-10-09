
#include <stdio.h>
#include <string.h>

#include "esp_bit_defs.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "eth_main.h"
#include "esp_http_client_example.h"
#include "mqtt_example.h"

static const char *TAG = "main";

#define GOT_IPV4 BIT0

EventGroupHandle_t eth_ev;

#if CONFIG_W5100_CUSTOM_SPI_TRANS
#	include "freertos/semphr.h"

#	include "w5100_spi.h"

SemaphoreHandle_t spi_mutex;

void spi_trans( spi_device_handle_t spi, uint32_t tx, uint32_t *rx )
{
	xSemaphoreTake( spi_mutex, portMAX_DELAY );
	ESP_ERROR_CHECK(
		spi_device_transmit( spi, &( spi_transaction_t ){ .length = 32, .tx_buffer = &tx, .rx_buffer = rx } ) );
	xSemaphoreGive( spi_mutex );
}
#endif

/** Event handler for Ethernet events */
static void eth_event_handler( void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	uint8_t mac_addr[ 6 ] = { 0 };
	/* we can get the ethernet driver handle from event data */
	esp_eth_handle_t eth_handle = *( esp_eth_handle_t * )event_data;

	switch ( event_id )
	{
		case ETHERNET_EVENT_CONNECTED:
			esp_eth_ioctl( eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr );
			ESP_LOGV( TAG, "Ethernet Link Up" );
			ESP_LOGV( TAG,
				"Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x",
				mac_addr[ 0 ],
				mac_addr[ 1 ],
				mac_addr[ 2 ],
				mac_addr[ 3 ],
				mac_addr[ 4 ],
				mac_addr[ 5 ] );
			break;
		case ETHERNET_EVENT_DISCONNECTED:
			ESP_LOGI( TAG, "Ethernet Link Down" );
			break;
		case ETHERNET_EVENT_START:
			ESP_LOGI( TAG, "Ethernet Started" );
			break;
		case ETHERNET_EVENT_STOP:
			ESP_LOGI( TAG, "Ethernet Stopped" );
			break;
		default:
			break;
	}
}

/** Event handler for IP_EVENT_ETH_GOT_IP */
static void got_ip_event_handler( void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data )
{
	ip_event_got_ip_t *event = ( ip_event_got_ip_t * )event_data;
	const esp_netif_ip_info_t *ip_info = &event->ip_info;

	ESP_LOGI( TAG, "Ethernet Got IP Address" );
	ESP_LOGI( TAG, "~~~~~~~~~~~" );
	ESP_LOGI( TAG, "ETHIP:" IPSTR, IP2STR( &ip_info->ip ) );
	ESP_LOGI( TAG, "ETHMASK:" IPSTR, IP2STR( &ip_info->netmask ) );
	ESP_LOGI( TAG, "ETHGW:" IPSTR, IP2STR( &ip_info->gw ) );
	ESP_LOGI( TAG, "~~~~~~~~~~~" );

	xEventGroupSetBits( eth_ev, GOT_IPV4 );
}

void tasklol( void *p )
{
	ESP_ERROR_CHECK( spi_bus_initialize( SPI3_HOST,
		&( spi_bus_config_t ){ .miso_io_num = GPIO_NUM_19,
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

	eth_ev = xEventGroupCreate();

	// Register user defined event handers
	ESP_ERROR_CHECK( esp_event_handler_register( ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL ) );
	ESP_ERROR_CHECK( esp_event_handler_register( IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL ) );

#if CONFIG_W5100_CUSTOM_SPI_TRANS
	ESP_ERROR_CHECK( !( spi_mutex = xSemaphoreCreateMutex() ) );

	set_spi_trans_cb( spi_trans );
#endif
	// Uncomment the initializer list below to enable static IPv4
	eth_main( &( struct eth_ifconfig ){
		.hostname = "w5100_esp32",
		// {
		// 	.ip.u8 = { 192, 168, 0, 220 },
		// 	.nm.u8 = { 255, 255, 255, 0 },
		// 	.gw.u8 = { 192, 168, 0, 1 },
		// 	.p_dns.u8 = { 1, 1, 1, 1 },
		// 	.s_dns.u8 = { 8, 8, 8, 8 },
		// 	.f_dns.u8 = { 8, 8, 4, 4 },
		// },
	} );

	xEventGroupWaitBits( eth_ev, GOT_IPV4, pdFALSE, pdTRUE, portMAX_DELAY );

	http_client_test();
	mqtt_example();

	vTaskDelete( NULL );
}

void app_main( void )
{
	xTaskCreate( tasklol, "tasklol", 8192, NULL, 1, NULL );
}