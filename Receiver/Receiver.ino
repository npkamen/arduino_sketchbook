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

    //convert incoming cm reading to float %
    tankpv = (257.0 - float(RX_buffer[1]*256 + RX_buffer[2]) + 20.0) / 257.0 * 100.0;
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
  
  //if(((currentTm - lastPrintTm) > 10000) || (msgRec))
  if(Serial.available() > 0)
  {
    msgRec=false;
  
    if(lastMsgTm==0)
    {
      Serial.println("Waiting for first data message...");
    }
    else
    {
      if (Serial.read() == char('s'))
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
      else
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
    lastPrintTm = millis();
    
    //Clear serial buffer if something new comes in
    while(Serial.available() > 0)
    {
      char t = Serial.read();
    }
    
  }
}

