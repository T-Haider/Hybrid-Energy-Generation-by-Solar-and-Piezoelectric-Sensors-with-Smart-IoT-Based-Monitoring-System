#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "z3dEdgCppxHw33MQgFLq9A3ZZgAC7DQO";
char ssid[] = "ZONG4G-FEC9";
char pass[] = "00637421";

#define EspSerial Serial
#define ESP8266_BAUD 115200
ESP8266 wifi(&EspSerial);
BlynkTimer timer;

int load1 = 39;
int load2 = 41;
int load3 = 43;

int load1_virtual = 49;
int load2_virtual = 51;
int load3_virtual = 53;

int load1_virtual_read, load2_virtual_read ,load3_virtual_read;

int voltage = 220;
float unit_rate = 25;
float wh, kwh, power, bill;
float current;
float current_sesnor_pin = A8;
float current1 = 0;

void setup()
{
  Serial.begin(9600);        //start serial communication at baud rate 9600

  pinMode(load1, OUTPUT);
  pinMode(load2, OUTPUT);
  pinMode(load3, OUTPUT);

  pinMode(load1_virtual, INPUT);
  pinMode(load2_virtual, INPUT);
  pinMode(load3_virtual, INPUT);
  pinMode(current_sesnor_pin, INPUT);

  digitalWrite(load1, HIGH);
  digitalWrite(load2, HIGH);
  digitalWrite(load3, HIGH);

  digitalWrite(load1_virtual, LOW);
  digitalWrite(load2_virtual, LOW);
  digitalWrite(load3_virtual, LOW);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);
  Blynk.begin(auth, wifi, ssid, pass);
  timer.setInterval(5000L, myTimerEvent);
}

void loop()
{
 Blynk.run();
 timer.run();

 blynk_read();
 power_calculate();
 
}

//----------------------------------------------------
void blynk_read()
{

 load1_virtual_read  = digitalRead(load1_virtual);
 load2_virtual_read  = digitalRead(load2_virtual);
 load3_virtual_read = digitalRead(load3_virtual);

   if (load1_virtual_read == HIGH)
   {
    digitalWrite(load1, LOW);
   }

   if (load1_virtual_read == LOW)
   {
    digitalWrite(load1, HIGH);
   }
//------------------
   if (load2_virtual_read == HIGH)
   {
    digitalWrite(load2, LOW);
   }

   if (load2_virtual_read == LOW)
   {
    digitalWrite(load2, HIGH);
   }
//-------------------
   if (load3_virtual_read == HIGH)
   {
    digitalWrite(load3, LOW);
   }

   if (load3_virtual_read == LOW)
   {
    digitalWrite(load3, HIGH);
   }    
}


//-----------------------------------------------------

void myTimerEvent()
{
  // Please don't send more that 10 values per second.
  wh = wh + power * ( 2000.0 / 3600000.0 );
  kwh = wh / 1000.0;
//  float units_per_two_sec = kwh / 216000.0;
  bill = kwh * unit_rate;
//  Serial.println(kwh,3);
  Blynk.virtualWrite(V1, bill);
  Blynk.virtualWrite(V2, current);
  Blynk.virtualWrite(V3, power);
}

//-----------------------------------------------------
void power_calculate()
{
 current = analogRead(current_sesnor_pin);
 current = current;
// current = current * 2.5;

 if (current > 100)
 {
 current1 = current;
 }
 
 if (current <= 100)
 {
  current = current1;
 }

if (current <= 103 && current >= 100)
 {
  current = 0.00;
 }
 
 if (current <= 111 && current > 104)
 {
  current = 0.27;
 }

 if (current <= 116 && current > 111)
 {
  current = 0.54;
 }

 if (current <= 125 && current > 118)
 {
  current = 0.81;
 }
 
 power = voltage * current;
 Serial.println(current);
 
}
