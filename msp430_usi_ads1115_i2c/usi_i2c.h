/* ------------------------------------------------------------------------------------------------------------------------------
    usi_i2c.h
    
    Copyright (C) 2017 Rajiv Biswas <rajiv.biswas55@gmail.com>
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
   ----------------------------------------------------------------------------------------------------------------------------- */



#include <stdint.h>

#define I2C_RESTART 	1<<8    /* repeated start */
#define I2C_READ		2<<8    /* read a byte */

// Call this with one of the USIDIV_* constants as a usi_clock_divider parameter, which will set the clock divider used
// for USI I2C communications. The usi_clock_source parameter should be set to one of the USISSEL* constants. Example:
// i2c_init(USIDIV_5, USISSEL_2) uses SMCLK/16.
void i2c_init(uint16_t usi_clock_divider, uint16_t usi_clock_source);

// Sends a command/data sequence that can include restarts, writes and reads. Every transmission begins with a START,
// and ends with a STOP so you do not have to specify that. Will busy-spin if another transmission is in progress. Note
// that this is interrupt-driven asynchronous code: you can't just call i2c_send_sequence from an interrupt handler and
// expect it to work: you risk a deadlock if another transaction is in progress and nothing will happen before the
// interrupt handler is done anyway. So the proper way to use this is in normal code. This should not be a problem, as
// performing such lengthy tasks as I2C communication inside an interrupt handler is a bad idea anyway.  wakeup_sr_bits
// should be a bit mask of bits to clear in the SR register when the transmission is completed (to exit LPM0: LPM0_BITS
// (CPUOFF), for LPM3: LPM3_bits (SCG1+SCG0+CPUOFF))
void i2c_send_sequence(uint16_t const * sequence, uint16_t sequence_length, uint8_t *received_data, uint16_t wakeup_sr_bits);

typedef enum i2c_state_enum {
  I2C_IDLE = 0,
  I2C_START = 2,
  I2C_PREPARE_ACKNACK = 4,
  I2C_HANDLE_RXTX = 6,
  I2C_RECEIVED_DATA = 8,
  I2C_PREPARE_STOP = 10,
  I2C_STOP = 12} i2c_state_type;

extern i2c_state_type i2c_state;

// Use this to check whether a previously scheduled I2C sequence has been fully processed.
inline unsigned int i2c_done() {
  return(i2c_state == I2C_IDLE);
}

