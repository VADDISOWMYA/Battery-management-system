#define BLYNK_TEMPLATE_ID "TMPL3LyHwVTV2"
#define BLYNK_TEMPLATE_NAME "vaddisowmya"
#define BLYNK_AUTH_TOKEN "K1XfCMsDT1OYwZtr5tJwkSUykwvSRwwj"
#include <WiFi.h> 
#define BLYNK_PRINT Serial
#include "ACS712.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

#define OLED_RESET  -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "nothing1";
char pass[] = "somu123456";






#define DHTPIN 18
#define DHTTYPE    DHT11

const int relaypin1 = 26;
const int relaypin2 = 25;
const int relaypin3 = 23;
int analogpin = 34;
int currentpin = 35;
int buzzerpin = 19;

int ADC1 = 0;
int ADC2 = 0;

int chargeled = 27;
int dischargeled = 5;
int safeled = 15;
int threatled = 4;
//float in_min = 3.3;
//float out_max = 100;
//float in_max = 8.4;
//float out_min = 0;

float A = 98.36943;
float xc=8.29504;
float k = -0.89432;






DHT dht(DHTPIN, DHTTYPE);
ACS712 sensor(ACS712_30A, 35);
void setup() 
{
  Serial.begin(115200);
  dht.begin();
  sensor.calibrate();
  pinMode(DHTPIN, OUTPUT);
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  pinMode(relaypin3, OUTPUT);
  pinMode(buzzerpin , OUTPUT);
  pinMode(chargeled, OUTPUT);
  pinMode(dischargeled, OUTPUT);
  pinMode(safeled, OUTPUT);
  pinMode(threatled, OUTPUT);
  pinMode(34,INPUT);
  pinMode(35,INPUT);
  Blynk.begin(auth, ssid, pass);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
    display.clearDisplay();
    delay(10);


}
void loop() {

Blynk.run();
float t = dht.readTemperature();
float h = dht.readHumidity();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
     }
   //Blynk.virtualWrite(V1,t);
   Serial.print("Temperature = ");
   Serial.print(t);
   Serial.println("C");
   delay(500);


//VOLTAGE
 
  ADC1 = analogRead(analogpin);
  float voltage = 0;
  if (ADC1 == 0)
   {
    voltage = 0;
    }
  else
  {
  float vout = ((ADC1*3.3)/ (4096));
 voltage = ((vout)*((30000+7500)/7500))+0.70;
 //Blynk.virtualWrite(V0,voltage);
  }
 
 Serial.print("ADC1 = ");
 Serial.println(ADC1);

   Serial.print("voltage = ");
   Serial.print(voltage,3);
   Serial.println("V");
   delay(1000);

   //current 

   ADC2 = analogRead(currentpin);
   Serial.print("ADC2 = ");
   Serial.println(ADC2);
    //float I = sensor.getCurrentDC();
    
  
  // Send it to serial
  //Serial.println(String("I = ") + I + " A");
  
   //float current = (((ADC2)*5 / (4096))-2.5) * 0.66;
    float current_vtg = (ADC2 *3.3)/4096;
   float I= (current_vtg )*0.66;
    //float current = 0.90;
    //Blynk.virtualWrite(V2,current);
   //Serial.print("current = ");
   //Serial.print(current,3);
   //Serial.println("A");
   delay(1000);
 
  //SOC
  //int SOC = (voltage-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
   float SOC = (100 - A*(1-exp(-k*(voltage-xc))));
   //Blynk.virtualWrite(V3,SOC);
   Serial.print("SOC= ");
   Serial.print(SOC);
   Serial.println("%");
   delay(1000);


  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("T =");
  display.setCursor(50,10 );
  display.println(t);
  display.setCursor(0, 30);
  display.println("V =");
  display.setCursor(50,30 );
  display.println(voltage);
  display.setCursor(0, 50);
  display.println("C =");
  display.setCursor(50,50 );
  display.println(I);
  display.display(); 
 
  




if (t > 35.00 || (voltage > 6.00 || voltage < 2) || (I > 1.00 || I < -1.00)) 

  {
      if (t > 35.00 && (voltage > 6.00 || voltage < 2) && (I >1.50 || I < -1.00)) 
      {
          //Blynk.logEvent("tempvoltcurrent_alert");
          digitalWrite(safeled, LOW);
          digitalWrite(threatled, HIGH);
          digitalWrite(relaypin1, HIGH);
          digitalWrite(relaypin2, LOW);
          digitalWrite(relaypin3, HIGH);
          //Blynk.virtualWrite(V4,LOW);
      }
      else if((t > 35.00)&& (voltage < 3.00 || I > 1.5 ))   // discharging
      {
          //Blynk.logEvent("tempvoltcurrent_alert");
          digitalWrite(relaypin1, HIGH);
          digitalWrite(relaypin2, LOW);
          digitalWrite(safeled, LOW);
          digitalWrite(threatled, HIGH);
          digitalWrite(dischargeled, HIGH);
          //Blynk.virtualWrite(V4,LOW);
      }
      else if((t > 35.00) && (I < -1.00 || voltage > 7.00))  //charging
         {
          //Blynk.logEvent("tempvoltcurrent_alert");
          digitalWrite(relaypin1, HIGH);
          digitalWrite(relaypin3, HIGH);
          digitalWrite(chargeled, HIGH);
          digitalWrite(safeled, LOW);
          digitalWrite(threatled, HIGH);
          //Blynk.virtualWrite(V4,LOW);
         }
      else if (t > 35.00) 
          {
            //Blynk.logEvent("temperature_alert");
            digitalWrite(threatled, HIGH);
            digitalWrite(relaypin1, HIGH);
            digitalWrite(safeled, LOW);
            //Blynk.virtualWrite(V4,LOW);
          
          } 
      else if ((voltage < 3.50 || I > 1.5 )) 
          {
           // Blynk.logEvent("voltage_alert");
            //Blynk.logEvent("current_alert");
            digitalWrite(dischargeled, HIGH);
            digitalWrite(threatled, HIGH);
            digitalWrite(relaypin2, LOW);
            digitalWrite(safeled, LOW);
            //Blynk.virtualWrite(V4,LOW);

          } 
       else if(I < -1.00 || voltage > 7.00)          //DISCHARHING
          {
            //Blynk.logEvent("voltage_alert");
            //Blynk.logEvent("current_alert");
            digitalWrite(chargeled, HIGH);
            digitalWrite(relaypin2, LOW);
            digitalWrite(threatled, HIGH);
            digitalWrite(relaypin3, HIGH);
            digitalWrite(safeled, LOW);
           // Blynk.virtualWrite(V4,LOW);
          }
      
        }
  
else
    {
      
            digitalWrite(safeled, HIGH);
            digitalWrite(dischargeled,HIGH);
            digitalWrite(threatled, LOW);
            digitalWrite(relaypin3,LOW);
            digitalWrite(chargeled, LOW);
            digitalWrite(relaypin2, HIGH);
            digitalWrite(relaypin1, LOW);
            //Blynk.virtualWrite(V4,HIGH);
    }
      
    

}
