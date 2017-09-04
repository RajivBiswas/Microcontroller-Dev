
#include "msp430g2452.h"

/*
 * main.c
 */
// Defining segments, Port 1
#define SEG1    BIT0
#define SEG2    BIT2
#define SEG3    BIT4

  // P1.7    P1.6      P1.5    P1.4     P1.3    P1.2     P1.1    P1.0
  //  2        4        7      12        N/C      9        11      8

  // P2.7    P2.6      P2.5    P2.4     P2.3    P2.2     P2.1    P2.0
  //  -        -        -       -        5      10       N/C      1

// Digits - 0 to 9, leaving aside 8, 9 & 12(off)
// For 0
// P2.3 - 1 Port 2 = 0x08 Port 1 = 0x00
// For 1
// P1.1 - 1 P1.7 - 1 Port 2 = 0x0f Port 1 = 0x82
// For 2
// P1.6 - 1 P2.2 - 1 Port 2 = 0x04 Port 1 = 0x40
// For 3
// P2.0 - 1 P2.2 - 1 Port 2 = 0x05 Port 1 = 0x00
// For 4
// P1.1 - 1 P1.7 - 1 P2.0 - 1 Port 2 = 0x01 Port 1 = 0x82
// For 5
// P1.5 - 1 P2.0 - 1 Port 2 = 0x01 Port 1 = 0x20
// For 6
// P1.5 - 1          Port 2 = 0x00 Port 1 = 0x20
// For 7
// P1.7 - 1 P2.0 - 1 P2.2 - 1 P2.3 - 1 Port 2 = 0x0f Port 1 = 0x80
// For 8
// Port 2 = 0x00 Port 1 = 0x00
// For 9
// P2.0 - 1 Port 2 = 0x01 Port 1 = 0x00

void decode7SegmentDisplay(unsigned char segment, unsigned char val)
{
    P1OUT  = 0x00;
    P2OUT  = 0x00;
    switch(val)
    {
    case 0x00:
              P2OUT = 0x08, P1OUT |= 0x00;
              break;
    case 0x01:
              P2OUT = 0x0F, P1OUT |= 0x82;
              break;
    case 0x02:
              P2OUT = 0x04, P1OUT |= 0x40;
              break;
    case 0x03:
              P2OUT = 0x05, P1OUT |= 0x00;
              break;
    case 0x04:
              P2OUT = 0x01, P1OUT |= 0x82;
              break;
    case 0x05:
              P2OUT = 0x01, P1OUT |= 0x20;
              break;
    case 0x06:
              P2OUT = 0x00, P1OUT |= 0x20;
              break;
    case 0x07:
              P2OUT = 0x0f, P1OUT |= 0x80;
              break;
    case 0x08:
              P2OUT = 0x00, P1OUT |= 0x00;
              break;
    case 0x09:
              P2OUT = 0x01, P1OUT |= 0x00;
              break;
    }

    P1OUT |= segment;
}

void main(void) {
    unsigned char i =0x00, j =0x00, k =0x00;
    int delay_x, delay_y, delay_z;
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    P1DIR |=  0xf7;
    P2DIR |=  0xfF;

    while(1)
    {
        while(1)
        {
            for (i = 0x00; i < 0x0A; i++)
            {
            for (delay_x=0; delay_x < 5; delay_x++)
                for (delay_y=0; delay_y < 5; delay_y++)
                    for (delay_z=0; delay_z < 25; delay_z++)
                    {
                        decode7SegmentDisplay( SEG1, i);
                        decode7SegmentDisplay( SEG2, j);
                        decode7SegmentDisplay( SEG3, k);
                    }
            }
            if (i == 0x0A)
                j++;
            if (j == 0x0A)
            {
                j = 0x00;
                k++;
            }
            if (k == 0x0A)
                break;
        }

        while (1)
        {
            if ((P1IN & 0x08) == 0x00)
            {
                i = 0x00;
                j = 0x00;
                k = 0x00;
                break;
            }
            decode7SegmentDisplay( SEG1, 0x09);
            decode7SegmentDisplay( SEG2, 0x09);
            decode7SegmentDisplay( SEG3, 0x09);
        }
    }
}
