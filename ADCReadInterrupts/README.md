 
 # Configuring 10-Bit ADC(Internal) of TI MSP430G2553 using ADC10 Interrupt & communicating using UART Interrupt of TI MSP430
 
 The Project here uses ADC10, 10-Bit Internal ADC Interrupt of TI 
 MSP430G2553 to sample Analogue values in the Interrupt routine,
 ```
__interrupt void ADC10ISR (void)
{
 	ADC10CTL0 &= ~ENC;
	EQVal1[0]   = ADC10MEM;
	val1 = ( EQVal1[0] >> 8 ) & 0x03;
	val2 = EQVal1[0] & 0xff;
	uart_send_ADCvalue( val2, val1 );
}
```
The EQVal1 array is an Unsigned Integer array of size 1 (4-Bytes) on
TI MSP430GXX platform. But since the ADC value would be in 10-bit, 1023
Max, hence we need to consider only 10 bits from the Least significant 
Bit. Thus, two unsigned byte variables val1 & val2 are considered here. \
val2 - stores the least 8-bits of the 10-Bit ADC Sample \
val1 - stores the most significant 2-bits of the 10-Bit ADC Sample
    
The ADC Channel that has been configured here is ADC Channel, A5, P1.5
```
ADC10AE0  = BIT5;                            // Select channel A5, P1.5
ADC10CTL0 = ADC10ON + REFON + ADC10SR + SREF0 + ADC10SHT_3 + ADC10IE;
ADC10CTL1 = INCH0 + INCH2 + ADC10DIV0 +ADC10DIV1 + CONSEQ_2;
ADC10CTL0 |= ADC10SC + ENC;                  // Start conversion
```    
Please be careful not to select ADC10 Analog Input A1 & ADC10 Analog 
Input A2, as we are using it for UART Tx & Rx.
    
From the ADC Interrupt routine we call the uart_send_ADCvalue routine,
which Unpacks the values in neat ASCII Formatted Data Arrays & then 
triggers the Tx, Transmit Interrupt, USCIAB0TX_VECTOR associated with 
USCI_A0.
    
The Program has been compiled & built on \
Code Composer Studio \
Version: 7.1.0.00016 
    
Connect your Analog Input +ve  to A5, P1.5 & -ve to the Gnd of your TI 
MSP 430G2 LaunchPad Board & connect a FTDI Serial UART Cable or PL203
with Rx/Tx connected to P1.1(Rx), P1.2(Tx), with Gnd & Vcc connected to
corresponding Vcc & Gnd of the TI MSP430G Launchpad Board.

Also please ensure to connect the RxD & TxDJumpers in Vertical Position,
& not in Horizontal Position, as shown in the attached png file.

Regards, \
Rajiv.
