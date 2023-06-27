#include <Wire.h>
#define BLYNK_PRINT Serial
//#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define ONE_WIRE_BUS D6
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BlynkTimer timer;

char auth[] = "SqfnydEarO8y4TGs1irgFbyCDH0hfrbT";
char ssid[] = "temp"; //Wifi name
char pass[] = "temp12345";  //Wifi Password
const byte interruptPin = D8; // Pin to set interrupt to
int interruptCounter = 0; // counter of interrupt executions
int bpm=0,SEC=0;
int buz=D0;
int irpin = D7;
int irvalue=0;
int current_Sec=0,last_Sec=0;
WidgetLED led1(V0);
   void setup() 
{
 Serial.begin(9600); /* begin serial for debug */
 Serial.print("Reading sensors......");
 Serial.print("ECG BPM AND TEMP");
 pinMode(interruptPin, INPUT_PULLUP);
 pinMode(buz, OUTPUT);
 Serial.println("Dallas Temperature IC Control Library Demo"); 
 lcd.begin();
    lcd.backlight();
    lcd.setCursor(2,0);
    lcd.print("Turn On Your");
    lcd.setCursor(2,1);
    lcd.print("Hotspot.........");
 Blynk.begin(auth, ssid, pass);
 delay(1000);
// timer.setInterval(1000L, getSendData);
 sensors.begin(); 
    attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);
 delay(1000);
}

void loop() 
{
     Blynk.run();
//  timer.run();
irvalue=digitalRead(irpin);

  Serial.println("Reading Sensors...");
sensors.requestTemperatures(); // Send the command to get temperature readings 
int T= sensors.getTempCByIndex(0); // Send the command to get temperature readings 
int T1= (T*1.8)+32;
 Serial.println("DONE"); 
  Serial.print("Temperature is: "); 
 Serial.print(sensors.getTempCByIndex(0));
  
current_Sec=millis();
SEC =current_Sec-last_Sec;
//Serial.print("Time:");
//Serial.println(SEC);

if(SEC>=30200)
{
last_Sec=millis();
  bpm=interruptCounter;
  Serial.print("BPM=");
  Serial.println(bpm);
  interruptCounter = 0;
  
 }
else
{
  delay(5000);
  bpm=0;
}
int bpm1=bpm*1.7;
 if(bpm1>140)
  {
    Blynk.notify("BPM IS HIGH");
    digitalWrite(buz,HIGH);
    delay(1000);
    digitalWrite(buz,LOW);
     delay(500);
  }
 else if(T1>102)
  {
    Blynk.notify("TEMPERATURE IS HIGH");
    digitalWrite(buz,HIGH);
    delay(1000);
    digitalWrite(buz,LOW);
     delay(500);
  }
   else if(T1<70)
  {
    Blynk.notify("TEMPERATURE IS LOW");
    digitalWrite(buz,HIGH);
    delay(1000);
    digitalWrite(buz,LOW);
     delay(500);
  }
  else 
  {
    digitalWrite(buz,LOW);
  }
   if (irvalue==1) {
    Blynk.notify("Bed Is Empty, Please Check Your Baby");
    led1.off();
    Serial.println("LED on V1: off");
  } else {
    led1.on();
    Serial.println("LED on V1: on");
  }
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("TEMP = ");
  lcd.setCursor(8,0);
  lcd.print(T1);
  lcd.setCursor(12,0);
  lcd.print("F");
   lcd.setCursor(1,1);
  lcd.print("BPM  = ");
  lcd.setCursor(7,1);
  lcd.print(bpm1);
  lcd.setCursor(12,1);
  lcd.print("bpm");
  Blynk.virtualWrite(V1, T1);
  Blynk.virtualWrite(V2, bpm1);
  
delay(1000);
  
}
void handleInterrupt() 
{
static unsigned long last_interrupt_time = 0;
unsigned long interrupt_time = millis();
// If interrupts come faster than 100ms, assume it's a bounce and ignore
if (interrupt_time - last_interrupt_time > 100)
{
interruptCounter++;
}
last_interrupt_time = interrupt_time;
//Serial.print("TIME:");
//Serial.println(millis()/1000);
}
