/* ------------------------------------------------------------------------------------------------------------------------------
    main.c
    main function showing how we can use USI I2C Library to READ/WRITE 
    to ADS1115. The Start Conversion till the Conversion end sequence
    in Single Shot Mode is as per the Datasheet of ADS1115(Refer 
    Sec 9.4.2, Pg. 19)
    
    Refer the READ/WRITE Timing Diagrams of ADS1115(Refer Sec 9.5.3,
    Pg. no. 22)
    
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

#include "msp430g2452.h"
#include "usi_i2c.h"
/*
 * main.c
 */

#define SLAVE_ADDRESS                    0x90
const uint8_t ads1115_conversion_reg   = 0x00;
const uint8_t ads1115_config_reg       = 0x01;
const uint8_t ads1115_lo_thresh_reg    = 0x02;
const uint8_t ads1115_hi_thresh_reg    = 0x03;

typedef struct AD1115_REG_WRITE_VAL
{
    uint8_t  reg;
    uint8_t WriteVal[ 0x02 ];
}AD1115_REG_WRITE_VAL_T;

AD1115_REG_WRITE_VAL_T  ad1115_config[ 0x03 ]=
{
    { 0x01    ,   {0x87, 0x63} },
    { 0x02    ,   {0x80, 0x00} },
    { 0x03    ,   {0x7F, 0xFF} }
};

void readAD1115(uint16_t* ad1115_read_reg, uint8_t* status)
{
    ad1115_read_reg[0x01] = ads1115_config_reg;
    i2c_send_sequence(ad1115_read_reg, 6, &status[0], 0x0010);
    LPM0;
    while(!i2c_done());

    ad1115_read_reg[0x01] = ads1115_conversion_reg;
    i2c_send_sequence(ad1115_read_reg, 6, &status[2], 0x0010);
    LPM0;
    while(!i2c_done());

    ad1115_read_reg[0x01] = ads1115_lo_thresh_reg;
    i2c_send_sequence(ad1115_read_reg, 6, &status[4], 0x0010);
    LPM0;
    while(!i2c_done());

    ad1115_read_reg[0x01] = ads1115_hi_thresh_reg;
    i2c_send_sequence(ad1115_read_reg, 6, &status[6], 0x0010);
    LPM0;
    while(!i2c_done());
}

void writeAD1115_configReg(uint16_t* ad1115_write_reg)
{
    ad1115_write_reg[0x01] = ad1115_config[0].reg;
    ad1115_write_reg[0x02] = ad1115_config[0].WriteVal[0] & 0x7F; // The MSB Bit is OR'red with 0x80 at time
    ad1115_write_reg[0x03] = ad1115_config[0].WriteVal[1];        // of WRITE, so no need to send MSB bit as HIGH, during I2C WRITE
    i2c_send_sequence(ad1115_write_reg, 4, 0, 0x0010);
    LPM0;
    while(!i2c_done());
}

void readAD1115_configReg(uint16_t* ad1115_read_reg, uint8_t* status)
{
    ad1115_read_reg[0x01] = ads1115_config_reg;
    i2c_send_sequence(ad1115_read_reg, 6, &status[0], 0x0010);
    LPM0;
    while(!i2c_done());
}

void readAD1115_convReg(uint16_t* ad1115_read_reg, uint8_t* status)
{
    ad1115_read_reg[0x01] = ads1115_conversion_reg;
    i2c_send_sequence(ad1115_read_reg, 6, status, 0x0010);
    LPM0;
    while(!i2c_done());
}

uint8_t startAD1115_conv()
{
    uint8_t  readConfig[2]      = {0x00, 0x00};
    uint16_t ad1115_read_reg[]  = {SLAVE_ADDRESS, 0x01, I2C_RESTART, (SLAVE_ADDRESS | 0x01), I2C_READ, I2C_READ};
    uint16_t ad1115_write_reg[] = {SLAVE_ADDRESS, 0x01, 0x00, 0x00};

    readAD1115_configReg( ad1115_read_reg, readConfig);

    // Assert OS Bit in Config Register
    ad1115_write_reg[0x02] = readConfig[0] | 0x80;
    ad1115_write_reg[0x03] = readConfig[1];
    i2c_send_sequence(ad1115_write_reg, 4, 0, 0x0010);
    LPM0;
    while(!i2c_done());

    readConfig[0] = 0x00, readConfig[1] = 0x00;
    readAD1115_configReg( ad1115_read_reg, readConfig);
    if ( (readConfig[0] >> 0x07) == 0x00 )
        return 0x01;
    else
        return 0x00;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    uint8_t  startConv;
    uint8_t  rVal[2] = {0x00, 0x00};
    uint16_t ad1115_read_reg[] = {SLAVE_ADDRESS, 0x01, I2C_RESTART, (SLAVE_ADDRESS | 0x01), I2C_READ, I2C_READ};
    //uint16_t ad1115_write_reg[] = {SLAVE_ADDRESS, 0x01, 0x00, 0x00};
    uint8_t Read_status[8]= {0x00};

    i2c_init(USIDIV_5, USISSEL_2);
    readAD1115(ad1115_read_reg, Read_status);

    while ( (startConv = startAD1115_conv()) == 0x00 );
        readAD1115_convReg(ad1115_read_reg, rVal);

    //writeAD1115_configReg(ad1115_write_reg);
    //readAD1115(ad1115_read_reg, Read_status);
}
