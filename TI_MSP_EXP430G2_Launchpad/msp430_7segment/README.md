# Interfacing TI MSP430 Launchpad with a packed, tri-paired, common-cathode 7-segment display
Recently, I came across this packed 7-segment display which struck me, as there were very few pins instead of a pack 
of Pins which usually features prominently in such displays which creates a terrible wiring nightmares when we try 
interfacing them with our Micro controllers. This 7-segment display is BT-M81DRD, which comes as shown below:

![Alt text](https://github.com/RajivBiswas/Microcontroller-Dev/blob/master/TI_MSP_EXP430G2_Launchpad/msp430_7segment/BT-M81DRD.png "BT-M81DRD")

To my surprise, there were no communication interfaces as i had thought. Instead there were only eight, 8-bit lines that were shared between the three 7-segments as was evident from the Data sheet diagram,

Thus, now few questions arose,

How to display any random data on all the segments simultaneously when all 
the segments digit lines were shared ?

Same Data would appear simultaneously on all these three segments leaving any scope for displaying any kind of random Data digits, which would defeat the purpose of getting this display.

This problem was very perplexing and needed some thought to solve it.

## The Solution 

The Solution to this problem came to me in the form of how Pictures are decoded in continuity by the Human Visual system. In effect, the process we know as persistence of vision plays a role in keeping the world from going pitch black every time we blink our eyes. Whenever light strikes the retina, the brain retains the impression of that light for about a 10th to a 15th of a second (depending on the brightness of the image, retinal field of view, and color) after the source of that light is removed from sight. This is due to a prolonged chemical reaction. As a result, the eye cannot clearly distinguish changes in light that occur faster than this retention period. The changes either go unnoticed or they appear to be one continuous picture to the human observer. 

In other words,I figured out that if I could ON/OFF the Display of these three 7-segments Fast enough for the eye, the eye would see it as continuous and would not be able to distinguish the blanking. This would create an impression of all the three segments being lit altogether and displaying the random digits on all the three segments. This would be the solution to my problem too. Viola i got it !

![Alt text](https://github.com/RajivBiswas/Microcontroller-Dev/blob/master/TI_MSP_EXP430G2_Launchpad/msp430_7segment/TI_MSP_Exp430G2_25.png "Interfacing Schematic between BT-M81DRD & Luanchpad")

Please follow the link to my Blog for more understanding & discussion,

```
https://www.linkedin.com/pulse/ti-msp-430-uart-i2c-adcs-rajiv-biswas?published=t
```

Regards,
Rajiv.
