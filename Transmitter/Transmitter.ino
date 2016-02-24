#include <ELECHOUSE_CC1101.h>
#include <NewPing.h>

#define TRIGGER_PIN  6  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     5  // Arduino pin tied to echo pin on the ultrasonic sensor.

//Set up sensor distance parameters
#define MAX_DISTANCE 300 //Distance for sensor to give up read if no response received by
#define TANK_HEIGHT 257 //Distance from sensor face to bottom of tank
#define MIN_DISTANCE 20 //Distance from sensor face to minimum measurement distance

#define size 6

byte TX_buffer[size]={0};
int i=0;
int battv=0;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  //Serial.begin(9600);
  ELECHOUSE_cc1101.Init();
 }

void loop()
{
  //Ping and condition tank level
  int uS = sonar.ping_median(30);
  int dist = uS / US_ROUNDTRIP_CM;
  float lvlpf = float(TANK_HEIGHT - dist) / float(TANK_HEIGHT - MIN_DISTANCE) * 100.0;

  if (lvlpf < 0.0) {
    lvlpf = 0.0;
  }
  else if (lvlpf > 100.0) {
    lvlpf = 100.0;
  }
  
  byte lvlp = lvlpf; 
  
  battv=readVcc();
  
  Serial.print(lvlp);
  Serial.print(' ');
  Serial.print(highByte(battv));
  Serial.print(' ');
  Serial.println(lowByte(battv));
  
  //Map data to array to be transmitted
  TX_buffer[0]=i;
  TX_buffer[1]=highByte(dist);
  TX_buffer[2]=lowByte(dist);
  TX_buffer[3]=lvlp;
  TX_buffer[4]=highByte(battv);
  TX_buffer[5]=lowByte(battv);
  
  //Send data
  ELECHOUSE_cc1101.SendData(TX_buffer,size);
  
  i++;
  
  //Wait for 60s before re-sampling and sending again
  delay(60000);
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
