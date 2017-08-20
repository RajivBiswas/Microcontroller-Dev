# Interfacing & Writing driver for a SCCB Interface Camera, OV7670 with Arduino

![Alt text](https://github.com/RajivBiswas/Microcontroller-Dev/blob/Arduino_AtMega/Arduino_AtMega/OV7670_with_AL422BFrameBuffer/Artikel4.png "Complete Setup")

I recently completed this project at my Home here.  Here’s a bare metal Camera
Sensor, OV7670 for which I have written the driver on my Arduino Board, Arduino 
Mega 2560 to control the various VIDEO Signals like HREF, PCLK, VSYNC, WREN, et 
al and VIDEO Control registers for Image Capture like the AWB(White Balance), 
AEC(Exposure Control), ABC(Black level correction), Denoising, Gamma level 
correction, Frame Rate, ADC, Gains, YUV Color matrices, Edge correction,  et al. 

This Camera Sensor, OV7670 comes with a FIFO Memory Buffer, AL422B.Now, the FIFO 
Memory Buffer Chip, AL422B captures the video image data from the main camera chip 
and we Read the Image Data from the FIFO & WRITE the Image Data in the form of a 
YUV File on the SD Card. 

For more understanding please follow the attached Word Document,
```
Interfacing&WritingDriverForA_SCCB_Interface_CameraOV7670withArduino.doc
```
Hope you like it. \
For more queries, please drop me a mail at: rajiv.biswas55@gmail.com

Regards, \
Rajiv.
