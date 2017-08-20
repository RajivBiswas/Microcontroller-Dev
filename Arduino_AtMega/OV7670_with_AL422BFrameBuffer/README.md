# Interfacing & Writing driver for a SCCB Interface Camera, OV7670 with Arduino

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

Hope you like it. 


Regards, \
Rajiv.
