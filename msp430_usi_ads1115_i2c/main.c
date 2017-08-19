

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
