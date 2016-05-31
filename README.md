# arduino_sketchbook
This is my Arduino sketchbook committed to github.  I may move it away from here at some stage but for the time being it is ok being here.  All code is in public domain.  Each project will have its own section here.



## Home Tank Level monitoring program.

### Programs Contained in this project
* tanklevel_receiver
* tanklevel_transmitter

### Equipment Used
* Arduino nano (clone) (2 off)
* CC1101 433MHz radio (2 off)
* JSN-SR04T Weatherproof ultrasonic level transducer
 
### Project Notes
* Modified ELECHOUSE_CC1101 library
 * Added RSSI and LQI outputs from ReceiveData function
 * Modified baud rate to 1200bps to increase sensitivity of receiver
