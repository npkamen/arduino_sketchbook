#include <ELECHOUSE_CC1101.h> // Data radio library
#include <NewPing.h> // Ultrasonic sensor library
#include <JeeLib.h> // Low power functions library

//Set up ultrasonic connection details
#define TRIGGER_PIN 6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN    5  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define PWR_PIN     4  // Arduino pin used to switch on level sensor (to save power when not in use).

//Set up sensor distance parameters
#define MAX_DISTANCE 300 //Distance for sensor to give up read if no response received (cm)
#define TANK_HEIGHT 257 //Distance from sensor face to bottom of tank (cm)
#define MIN_DISTANCE 30 //Distance from sensor face to minimum measurement distance (cm)

//Set up transmit parameters
#define TX_SIZE 61
#define TX_ADDR 10
byte TX_buffer[TX_SIZE]={0};
long lastTransmit;

//Set up receive parameters
#define RX_SIZE 61
byte RX_buffer[RX_SIZE]={0};
byte rx_size,rx_i;

//Other misc parameters
byte bTransmitLevel;
byte bSleepExpired;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

ISR(WDT_vect) { Sleepy::watchdogEvent(); } // Setup the watchdog


void setup() {
  Serial.begin(9600);
  ELECHOUSE_cc1101.Init();
  pinMode(PWR_PIN, OUTPUT);
  attachInterrupt(0,ELECHOUSE_CC1101_RevData,FALLING);
  lastTransmit = millis()
 }

void loop() {
  //Snapshot current time to be re-used during loop cycle
  long currentTime = millis();
  
  //Check for received message and map data to relevant locations
  if (RX_buffer[0] <> 0) {
    Serial.print("Message received from node ");
    Serial.print(RX_buffer[0]);
    Serial.println(".");
    
    if (RX_buffer[0] = 1) { //Message received from base receiver
      RX_buffer[0] = 0;
      
      if (bitRead(RX_buffer[1],0)) { //Check for refresh command
        Serial.println("Refresh command received")
        bTransmitLevel = 1;
      }
      
      
    }
  }
  
  if (bTransmitLevel || (bSleepExpired = 1) {
    sleep_pwron = 0;
    bTransmitLevel = 0;
    //Read tank level and voltage
    tankLevel()
    
    //Map data to array to be transmitted
    TX_buffer[0]=TX_ADDR;
    TX_buffer[1]=highByte(dist);
    TX_buffer[2]=lowByte(dist);
    TX_buffer[3]=highByte(lvlp);
    TX_buffer[4]=lowByte(lvlp);
    TX_buffer[5]=highByte(battv);
    TX_buffer[6]=lowByte(battv);
   
    //Transmit data
    ELECHOUSE_cc1101.SendData(TX_buffer,TX_SIZE);
    
    //Update last transmission time
    lastTransmit = currentTime;
  }

  sleep_pwron=Sleepy::loseSomeTime(60000);  //Wait for 60s before re-sampling and sending again

}

void tankLevel() {
  //Ping and condition tank level
    Serial.println("Powering up level sensor");
  digitalWrite(PWR_PIN, HIGH);
    Serial.println("Waiting 1s for sensor to stabilise");
  Sleepy::loseSomeTime(1000);
    Serial.println("Sensor warmup complete, reading value....");
  long uS = sonar.ping_median(30);
    Serial.print("Sensor ping time is ");
    Serial.print(uS);
    Serial.println("uS.")
  //Read battery volts straight after ping to see if there is any voltage drop
    Serial.println("Reading Battery Volts....");
  int battv=readVcc();
    Serial.print("Battery Volts is ");
    Serial.print(battv);
    Serial.println("mV.");
    Serial.println("Switching level sensor off.");
  digitalWrite(PWR_PIN, LOW);
  
  long dist = (float(uS) / float(US_ROUNDTRIP_CM)) * 10.0; //store distance in mm
    Serial.print("Sensor distance is ");
    Serial.print(dist);
    Serial.println("mm.");
  int lvlp = float((TANK_HEIGHT*10) - dist) / float((TANK_HEIGHT*10) - (MIN_DISTANCE*10)) * 100.0; //Pack level into integer with 2dp
    Serial.print("Tank level is ");
    Serial.print(float(lvlp)/100.0,2); //Scale the level for serial print
    Serial.println("%.");

  if (lvlp < 0) {
    lvlp = 0;
  }
  else if (lvlp > 10000) {
    lvlp = 10000;
  }
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

void ELECHOUSE_CC1101_RevData()
{
  rx_size=ELECHOUSE_cc1101.ReceiveData(RX_buffer);
  Serial.print("New incoming message received: ");
  for(rx_i=0;rx_i<rx_size;rx_i++)
  {
    Serial.print(RX_buffer[i],DEC);
    Serial.print(' ',BYTE);
  }
  Serial.println("");
  ELECHOUSE_cc1101.SetReceive();
}
