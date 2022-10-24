The libraries that the IC tester uses.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**SD library**

This library needs to be referenced to directly or you need to override the one that comes with the arduino IDE. Placing it into you library folder in documents isn't enough!!! 

for Arduino IDE 2 on Windows 10 it can be found here

C:\Users\UserName\AppData\Local\Arduino15\libraries
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**TFT Libraries**

Sadly there are plenty of different screens and controllers being sold as the same thing.

I have only acquired and tested the SPFD5408 variants and even they don't work exactly the same... :[

1# If your screen only shows white after you have uploaded the sketch you probably need the other one. as in the adafruit type instead of the SPFD one or vice versa

2# If you have graphics on your screen after uploading but touch doesn't work and/or is inverted then you need to read the manual which goes into more depth on the subject. 
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Kicksort library

The alphabetical sorting library used with the IC tester. Put it in your libraries folder Documents/Arduino/libraries

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
