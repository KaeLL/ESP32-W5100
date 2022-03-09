/* MQTT over SSL Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "esp_tls.h"
#include "esp_ota_ops.h"
#include <sys/param.h>

#include "mqtt_example.h"

static const char *TAG = "MQTTS_EXAMPLE";

#if CONFIG_BROKER_CERTIFICATE_OVERRIDDEN == 1
static const uint8_t mqtt_eclipseprojects_io_pem_start[] =
	"-----BEGIN CERTIFICATE-----\n" CONFIG_BROKER_CERTIFICATE_OVERRIDE "\n-----END CERTIFICATE-----";
#else
extern const uint8_t mqtt_eclipseprojects_io_pem_start[] asm( "_binary_mqtt_eclipseprojects_io_pem_start" );
#endif
extern const uint8_t mqtt_eclipseprojects_io_pem_end[] asm( "_binary_mqtt_eclipseprojects_io_pem_end" );

//
// Note: this function is for testing purposes only publishing part of the active partition
//       (to be checked against the original binary)
//
static void send_binary( esp_mqtt_client_handle_t client )
{
	spi_flash_mmap_handle_t out_handle;
	const void *binary_address;
	const esp_partition_t *partition = esp_ota_get_running_partition();
	esp_partition_mmap( partition, 0, partition->size, SPI_FLASH_MMAP_DATA, &binary_address, &out_handle );
	// sending only the configured portion of the partition (if it's less than the partition size)
	int binary_size = MIN( CONFIG_BROKER_BIN_SIZE_TO_SEND, partition->size );
	int msg_id = esp_mqtt_client_publish( client, "/topic/binary", binary_address, binary_size, 0, 0 );
	ESP_LOGI( TAG, "binary sent with msg_id=%d", msg_id );
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler( void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data )
{
	ESP_LOGD( TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id );
	esp_mqtt_event_handle_t event = event_data;
	esp_mqtt_client_handle_t client = event->client;
	int msg_id;
	switch ( ( esp_mqtt_event_id_t )event_id )
	{
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI( TAG, "MQTT_EVENT_CONNECTED" );
			msg_id = esp_mqtt_client_subscribe( client, "/topic/qos0", 0 );
			ESP_LOGI( TAG, "sent subscribe successful, msg_id=%d", msg_id );

			msg_id = esp_mqtt_client_subscribe( client, "/topic/qos1", 1 );
			ESP_LOGI( TAG, "sent subscribe successful, msg_id=%d", msg_id );

			msg_id = esp_mqtt_client_unsubscribe( client, "/topic/qos1" );
			ESP_LOGI( TAG, "sent unsubscribe successful, msg_id=%d", msg_id );
			break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGI( TAG, "MQTT_EVENT_DISCONNECTED" );
			break;

		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGI( TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id );
			msg_id = esp_mqtt_client_publish( client, "/topic/qos0", "data", 0, 0, 0 );
			ESP_LOGI( TAG, "sent publish successful, msg_id=%d", msg_id );
			break;
		case MQTT_EVENT_UNSUBSCRIBED:
			ESP_LOGI( TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id );
			break;
		case MQTT_EVENT_PUBLISHED:
			ESP_LOGI( TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id );
			break;
		case MQTT_EVENT_DATA:
			ESP_LOGI( TAG, "MQTT_EVENT_DATA" );
			printf( "TOPIC=%.*s\r\n", event->topic_len, event->topic );
			printf( "DATA=%.*s\r\n", event->data_len, event->data );
			if ( strncmp( event->data, "send binary please", event->data_len ) == 0 )
			{
				ESP_LOGI( TAG, "Sending the binary" );
				send_binary( client );
			}
			break;
		case MQTT_EVENT_ERROR:
			ESP_LOGI( TAG, "MQTT_EVENT_ERROR" );
			if ( event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT )
			{
				ESP_LOGI( TAG, "Last error code reported from esp-tls: 0x%x", event->error_handle->esp_tls_last_esp_err );
				ESP_LOGI( TAG, "Last tls stack error number: 0x%x", event->error_handle->esp_tls_stack_err );
				ESP_LOGI( TAG,
						  "Last captured errno : %d (%s)",
						  event->error_handle->esp_transport_sock_errno,
						  strerror( event->error_handle->esp_transport_sock_errno ) );
			}
			else if ( event->error_handle->error_type == MQTT_ERROR_TYPE_CONNECTION_REFUSED )
			{
				ESP_LOGI( TAG, "Connection refused error: 0x%x", event->error_handle->connect_return_code );
			}
			else
			{
				ESP_LOGW( TAG, "Unknown error type: 0x%x", event->error_handle->error_type );
			}
			break;
		default:
			ESP_LOGI( TAG, "Other event id:%d", event->event_id );
			break;
	}
}

static void mqtt_app_start( void )
{
	const esp_mqtt_client_config_t mqtt_cfg = {
		.uri = CONFIG_BROKER_URI,
		.cert_pem = ( const char * )mqtt_eclipseprojects_io_pem_start,
	};

	ESP_LOGI( TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size() );
	esp_mqtt_client_handle_t client = esp_mqtt_client_init( &mqtt_cfg );
	/* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
	esp_mqtt_client_register_event( client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL );
	esp_mqtt_client_start( client );
}

void mqtt_example( void )
{
	ESP_LOGI( TAG, "[APP] Startup.." );
	ESP_LOGI( TAG, "[APP] Free memory: %d bytes", esp_get_free_heap_size() );
	ESP_LOGI( TAG, "[APP] IDF version: %s", esp_get_idf_version() );

	mqtt_app_start();
}
