 #include <ELECHOUSE_CC1101.h>
 
 void setup()
{
  Serial.begin(9600);
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.SetReceive();
}

byte RX_buffer[61]={0};
byte RX_rssi=0;
byte RX_lqi=0;
byte size,i,flag;

float battv=0.0;
float tankpv=0.0;
int tankcm=0;
long lastMsgTm=0;
long lastPrintTm=0;
long currentTm=0;
int rssi=0;
int quality=0;
boolean msgRec=false;

void loop()
{
  
  currentTm=millis();
  
  if(ELECHOUSE_cc1101.CheckReceiveFlag())
  {
    msgRec=true;
    size=ELECHOUSE_cc1101.ReceiveData(RX_buffer, RX_rssi, RX_lqi);

    //convert RSSI to actual figure
    if(RX_rssi>=128)
    {
      rssi=(RX_rssi-256)/2-74;
    }
    else
    {
      rssi=(RX_rssi/2)-74;
    }

    //condition tank level in cm coming back from tank sensor
    tankcm = RX_buffer[1]*256+RX_buffer[2];
    if (tankcm < 20)
    {
      tankcm = 20;
    }

    //convert incoming cm reading to float %
    tankpv = float(237 - tankcm - 20) / 237.0 * 100.0;
    if (tankpv < 0.0)
    {
     tankpv = 0.0;
    }
    else if (tankpv > 100.0)
    {
     tankpv = 100.0;
  }

    //scale LQI to 0-100% quality figure
    quality=(100 - ((RX_lqi - 128)/128*100));
    
    //convert battery voltage back to int
    battv=float(RX_buffer[4]*256 + RX_buffer[5])/1000.0;
    
    ELECHOUSE_cc1101.SetReceive();
    
    lastMsgTm=currentTm;
  }
  
  if(Serial.available() > 0)
  {
    char cmd=Serial.read();
    msgRec=false;
  
    if(lastMsgTm==0)
    {
      Serial.println("Waiting for first data message...");
    }
    else if (((currentTm-lastMsgTm)/1000) > 300)
    {
      Serial.println("Comms Failure with Sensor");
    }
    else
    {
      if (cmd == char('s'))
      {
        
        Serial.print("Status - Level:");
        Serial.print(tankpv);
        Serial.print("% Sensor Distance:");
        Serial.print(RX_buffer[1]*256 + RX_buffer[2]);
        Serial.print("cm Signal:");
        Serial.print(rssi);
        Serial.print("dBm Quality:");
        Serial.print(quality);    
        Serial.print("% Battery Volts:");
        Serial.print(battv);
        Serial.print("V Last Message Received:");
        Serial.print((currentTm-lastMsgTm)/1000);
        Serial.println("s ago.");
      }
      else if (cmd == char('c'))
      {
        Serial.print("Level:");
        Serial.print(tankpv);
        Serial.print(" RSSI:");
        Serial.print(rssi);
        Serial.print(" Quality:");
        Serial.print(quality);
        Serial.print(" BattV:");
        Serial.print(battv);
        Serial.println("");
      }
    }
    
    //Clear serial buffer if something new comes in during serial transfer of data
    while(Serial.available() > 0)
    {
      char t = Serial.read();
    }
    
  }
}

