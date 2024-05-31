
#pragma once

#include <stdint.h>

void w5100_spi_init( void );
void w5100_spi_deinit( void );
void w5100_ll_hw_reset( void );
void w5100_read( const uint16_t addr, uint8_t *const data_rx, const uint32_t size );
void w5100_write( const uint16_t addr, const uint8_t *const data_tx, const uint32_t size );
