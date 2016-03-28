# dryplain_tanklevel
Home Tank Level monitoring program.

Equipment Used:
* Arduino nano (clone) (2 off)
* CC1101 433MHz radio (2 off)
* JSN-SR04T Weatherproof ultrasonic level transducer
 
This project contains:
* Arduino code for Receiver and Transmitter
* Modified ELECHOUSE_CC1101 library
* Added RSSI and LQI outputs from ReceiveData function
* Modified baud rate to 1200bps to increase sensitivity of receiver
