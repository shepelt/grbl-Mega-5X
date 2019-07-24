/*
  current_control.c - methods for setting DigiPots over SPI
  Part of Grbl

  Copyright (c) 2012-2016 Sungeun K. Jeon for Gnea Research LLC

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "current_control.h"
#include "grbl.h"

#ifdef HAS_DIGIPOTS

void current_init()
{
    static const uint8_t digipot_motor_current[] = DIGIPOT_MOTOR_CURRENT;
    // Set the SS pin to high
    SPI_DDR |= (1<<SS_BIT);

    // Set DIGIPOTSS_PIN as output
    DIGIPOTSS_DDR |= 1<<DIGIPOTSS_BIT;
    // Set MOSI, SCK as Output
    SPI_DDR |= (1<<MOSI_BIT)|(1<<SCK_BIT);

    // Initialize SPI as master
    SPCR = (1<<SPE)|(1<<MSTR);

    uint8_t i;
    for (i = 0; i < N_AXIS; i++) {
        set_current(i, digipot_motor_current[i]);
    }
}

inline static uint8_t spi_transfer(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & (1<<SPIF))); // wait
    return SPDR;
}

void set_current(uint8_t motor, uint8_t value)
{
    const uint8_t digipot_ch[] = DIGIPOT_CHANNELS;

    // Take the SS pin low to select the chip
    DIGIPOTSS_PORT &= ~(1<<DIGIPOTSS_BIT);
    // Send the address and value via SPI
    spi_transfer(digipot_ch[motor]);
    spi_transfer(value);
    // Take the SS pin high to de-select the chip
    DIGIPOTSS_PORT |= (1<<DIGIPOTSS_BIT);
}

#endif // HAS_DIGIPOTS
