# BBB_P10
C library for BeagleBone Black to control Led Display Panels.
<br>
<img height="400" src="https://github.com/joek85/BBB_P10/blob/master/BBB_P10.jpg?raw=true" />
<br>

This is a library for BeagleBone Black to control any led display panel with 32X16 dimensions,tested on 3 panels with snake clocking.
My beaglebone has the default Angstrom distro installed :Linux beaglebone 3.8.13 #1 SMP Wed Sep 4 09:09:32 CEST 2013 armv7l GNU/Linux , with gcc version 4.7.3 20130205 (prerelease) (Linaro GCC 4.7-2013.02-01),so the library is cross-compiled with gcc-4.7.

This library is controling the I/O registers using BBBIO library,Thanks to https://github.com/VegetableAvenge/BBBIOlib.
The library is using the standard GPIO pins,if you want to use the library with SPI let me know i can help you to get started configuring the SPI on beaglebone.

There is a image for the pinout connections on the panel to use it as a reference when connecting it to the bone.
Here is a demo on Youtube : https://www.youtube.com/watch?v=CJcga0YVDdY
Any questions,feel free to open an issue to help you more.Thank you,enjoy.
