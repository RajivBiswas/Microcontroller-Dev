#include <msp430.h> 

/*
 * main.c
 */
#include "msp430g2553.h"

unsigned char data[15], tx_count;

void delay(unsigned int x)
{
    while(x--);
}

void uart_init( void ) {
    /**
     * 1. Set UCSWRST
     * 2. Initialize all USCI registers
     * 3. Configure ports
     * 4. Clear UCSWRST
     * 5. Enable interrupts via UCxRXIE and/or UCxTXIE
     */

    tx_count = 0x00;

    // (1) Set state machine to the reset state
    UCA0CTL1 = UCSWRST;

    // (2) Initialize USCI registers
    UCA0CTL1 |= UCSSEL_2;                                 // CLK = SMCLK
    UCA0BR0 = 104;                                        // 1MHz/9600 = 104
    UCA0BR1 = 0x00;                                       //
    UCA0MCTL = UCBRS0;                                    // Modulation UCBRSx = 1

    // (3) Configure ports
    P1SEL = BIT1 + BIT2 ;                                 // P1.1 = RXD, P1.2=TXD
    P1SEL2= BIT1 + BIT2 ;                                 // P1.1 = RXD, P1.2=TXD

    // (4) Clear UCSWRST flag
    UCA0CTL1 &= ~UCSWRST;                                 // Initialize USCI state machine
}

void uart_send_ADCvalue( unsigned char val, unsigned char valMSB )
{
    int i, convVal = 0x00, units = 0x00, tens = 0x00, hundreds = 0x00, thousand = 0x00;

    for (i = 0; i < 2; i++)
        convVal += ( ((valMSB & (0x01 << i)) >> i) << (i+8) );

    for (i = 0; i < 8; i++)
        convVal += ( ((val & (0x01 << i)) >> i) << i );

        if ( convVal > 1000 )
        {
            while( convVal > 1000)
            {
                convVal  -= 1000;
                thousand += 0x01;
            }
        }

        if ( convVal > 100 )
        {
            while( convVal > 100)
            {
                convVal  -= 100;
                hundreds += 0x01;
            }
        }

        if ( convVal > 10 )
        {
            while( convVal > 10)
            {
                 convVal  -= 10;
                 tens     += 0x01;
            }
            units = convVal;
        }
        tx_count = 0;
        data[0]  = 65;  // 'A'
        data[1]  = 68;  // 'D'
        data[2]  = 67;  // 'C'
        data[3]  = 32;  // Space
        data[4]  = 118; // 'v'
        data[5]  = 97;  // 'a'
        data[6]  = 108; // 'l'
        data[7]  = 58;  // ':'
        data[8]  = 32;  // Space
        data[9]  = thousand + 48;
        data[10] = hundreds + 48;
        data[11] = tens + 48;
        data[12] = units + 48;
        data[13] = 13;
        data[14] = 10;

        IE2 |= UCA0TXIE;                                      // enable interrupts to start the transfer via UART
        _BIS_SR(GIE);                                         // Enter interrupt
}

#if 0
// This routine does the sending.  An interrupt is triggered each time
// the UART is ready to transmit the next character.
void USCI_A_tx (void) __attribute__((interrupt(USCIAB0TX_VECTOR)));
void USCI_A_tx (void)
{
    UCA0TXBUF = data[ tx_count++ ];    // Load the character
    // After the 6 characters of the message are sent, we're done.
    if( tx_count >= 6 ) {
        IE2 &= ~UCA0TXIE;       // disable this routine so it won't be called again
        tx_count = 0;           // reset the count for the next transmission
    }
}
#endif

volatile unsigned int EQVal1[1];     // volatile to prevent optimization
unsigned char val1, val2;

#pragma vector=ADC10_VECTOR
__interrupt void ADC10ISR (void)
{
    ADC10CTL0 &= ~ENC;
    EQVal1[0]   = ADC10MEM;
    val1 = ( EQVal1[0] >> 8 ) & 0x03;
    val2 = EQVal1[0] & 0xff;

    uart_send_ADCvalue( val2, val1 );
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void usart0_tx (void)
{
    UCA0TXBUF = data[ tx_count++ ];    // Load the character
    // After the 6 characters of the message are sent, we're done.
    if( tx_count >= 15 )
    {
        IE2 &= ~UCA0TXIE;             // disable this routine so it won't be called again
        tx_count = 0;                 // reset the count for the next transmission
        ADC10CTL0 |= ADC10SC+ENC;     // after Transmit complete, now enable the ADC, so that ADC Conversion Interrupt is generated
        __bis_SR_register( GIE );
    }
}

void main(void)
{
    //volatile unsigned int EQVal[7];     // volatile to prevent optimization
    WDTCTL = WDTPW + WDTHOLD;           // Stop Watchdog timer

    // The UART settings used depend on a good 1MHz clock
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;

    P1DIR &= ~BIT3;                     // P1.3 is an input pin
    P1IE |= BIT3;                       // Switch S2 triggers an interrupt
    P1IES |= BIT3;                      // on falling edge

    uart_init();
    ADC10AE0  = BIT5;                            // Select channel A5, P1.5
    ADC10CTL0 = ADC10ON + REFON + ADC10SR + SREF0 + ADC10SHT_3 + ADC10IE;
    ADC10CTL1 = INCH0 + INCH2 + ADC10DIV0 +ADC10DIV1 + CONSEQ_2;
    ADC10CTL0 |= ADC10SC + ENC;                  // Start conversion
    __bis_SR_register( GIE );

    while( 1 ) {

#if 0
        while( ADC10CTL1 & ADC10BUSY )
        {
            EQVal[0]   = ADC10MEM;                // Read the Conversion Data, at Falling edge of STROBE Signal
            ADC10CTL0 &= ~ENC;
        }
#endif

    }
}
