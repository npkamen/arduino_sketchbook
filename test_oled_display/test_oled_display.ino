/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

boolean updateDisplay;
int sigBars;
long startTime = millis();
long upTime;
long currentTime;
long lastUpdateTime;

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
//  display.display();
//  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  //  text display tests
//  display.setTextSize(1);
//  display.setTextColor(WHITE);
//  display.setCursor(0,0);
//  display.println("Hello, world!");
//  display.display();
//  delay(2000);
//  display.clearDisplay();

}


void loop() {

  currentTime = millis();
  
  upTime = (currentTime - startTime)/1000; 
  
  if(Serial.available() > 0)
  {
    char cmd=Serial.read();
    
    int newBars = int(cmd)-48;
    
    Serial.println(newBars);
    
    if ((newBars >= 0) & (newBars <= 4))
    {
      sigBars = newBars;
      updateDisplay = true;
    }
    
    while(Serial.available() > 0)
    {
      char t = Serial.read();
    }
  }
  
  if ((currentTime - lastUpdateTime) > 1000) {
    updateDisplay = true;
  }  
  
  if (updateDisplay) {
    displaySignal(sigBars);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,56);
    display.println(upTime);
    updateDisplay = false;
    lastUpdateTime = currentTime;
    display.display();
    display.clearDisplay();
    
  }
  
}

void displaySignal(int numBars) {
  // Display signal symbol
  display.drawTriangle(0,0,8,0,4,4,1);
  display.fillRect(4,5,1,5,1);
  if (numBars >= 1) { display.fillRect(6,8,1,2,1); }
  if (numBars >= 2) { display.fillRect(8,6,1,4,1); }
  if (numBars >= 3) { display.fillRect(10,4,1,6,1); }
  if (numBars >= 4) { display.fillRect(12,2,1,8,1); }
  delay(1);
}
