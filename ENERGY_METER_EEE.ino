#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL3KJEUXnYR"
#define BLYNK_TEMPLATE_NAME "SMART ELETRIC METER"
#define BLYNK_AUTH_TOKEN "n04MI9R9xxtMIJt8hTjozJ8f3tn2Jbgq"
#include "EmonLib.h"  
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <BigFont01_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); 
BigFont01_I2C     big(&lcd);
EnergyMonitor emon;
#define vCalibration 40
#define currCalibration 1
char auth[] = BLYNK_AUTH_TOKEN;
int totalColumns = 20;
int totalRows    =4;
BlynkTimer timer;
extern "C" 
{

  uint8_t temprature_sens_read(); 
}

uint8_t temprature_sens_read();
BLYNK_WRITE(V7)
{
  int pinValue=param.asInt();
  digitalWrite(15,pinValue);
} 
BLYNK_WRITE(V6)
{
  int pinValue=param.asInt();
  digitalWrite(5,pinValue);
} 

char ssid[] = "ABK";
char pass[] = "ABK12345";
 
float kWh = 0;
float RS = 0;
unsigned long lastmillis = millis();

 String scrollingMessage = " ELECTRICAL AND ELECTRONICS DEPARTMENT";
 void scrollMessage(int row, String message, int delayTime, int totalColumns)
{
 for (int i=0; i < totalColumns; i++)

{
 message = " " + message;  
 
} 
 message = message + " "; 
 for (int position = 0; position < message.length(); position++) {
 lcd.setCursor(0, row);
 lcd.print(message.substring(position, position + totalColumns));
    delay(300);
 }
}

void myTimerEvent() {

    emon.calcVI(20, 2000);
    Serial.print("Vrms: ");
    Serial.print(emon.Vrms, 2);
    Serial.print("V");
    
    Serial.print("\tIrms: ");
    Serial.print(emon.Irms, 4);
    Serial.print("A");
    
    Serial.print("\tPower: ");
    Serial.print(emon.apparentPower, 4);
    Serial.print("W");
    
    Serial.print("\tkWh: ");
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
    RS =(kWh*4);
    Serial.print(kWh, 4);
    Serial.println("kWh");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("VOLTAGE(rms):");
    lcd.print(emon.Vrms, 2);
    lcd.print("V");
    lcd.setCursor(0, 1);
    lcd.print("CURRENT(rms):");
    lcd.print(emon.Irms, 4);
    lcd.print("A");
    lcd.setCursor(0, 2);
    lcd.print("POWER    :");
    lcd.print(emon.apparentPower, 4);
    lcd.print("W");
    lcd.setCursor(0, 3);
    lcd.print("KWh      :");
    lcd.print(kWh, 4);
    lcd.print("W");
    delay(2500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("AMOUNT     :");
    lcd.print(RS); 
    lcd.print("rs");
    lcd.setCursor(0,1);
    lcd.print("TEMPERATURE:");
    lcd.print((temprature_sens_read() - 32) / 1.8); 
    lcd.print(".C");
    delay(2500);

    lastmillis = millis();

    Blynk.virtualWrite(V0, emon.Vrms);
    Blynk.virtualWrite(V1, emon.Irms);
    Blynk.virtualWrite(V2, emon.apparentPower);    
    Blynk.virtualWrite(V3, kWh);
    Blynk.virtualWrite(V4, RS);
    Blynk.virtualWrite(V8, (temprature_sens_read() - 32) / 1.8);
}
 
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.begin();
  big.begin();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write ('  BigFont01_I2C ');
  big.writechar(0,0,'I');
  big.writechar(0,3,'0');
  big.writechar(0,6,'T');
  big.writechar(2,5,'B');
  big.writechar(2,8,'A');
  big.writechar(2,11,'S');
  big.writechar(2,14,'E');
  big.writechar(2,17,'D');
  delay(2000);
  lcd.clear();
  big.writechar(1,2,'S');
  big.writechar(1,5,'M');
  big.writechar(1,10,'A');
  big.writechar(1,13,'R');
  big.writechar(1,16,'T');
  delay(2000);
  lcd.clear();
  big.writechar(0,0,'E');
  big.writechar(0,3,'N');
  big.writechar(0,7,'E');
  big.writechar(0,10,'R');
  big.writechar(0,13,'G');
  big.writechar(0,16,'Y');
  big.writechar(2,2,'M');
  big.writechar(2,7,'E');
  big.writechar(2,10,'T');
  big.writechar(2,13,'E');
  big.writechar(2,16,'R');
  delay(2000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GROUP NUMBER:I");
  lcd.setCursor(0,1);
  lcd.print("AMAL KRISHNA A");
  lcd.setCursor(0,2);
  lcd.print("ALBIN GEORGE,AKASH B");
  lcd.setCursor(0,3);
  lcd.print("AMBADI V,ANANDHU B S");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("GUIDED BY:");
  lcd.setCursor(0,1);
  lcd.print("MRS ATHIRA CHANDRAN");
  big.writechar(2,1,'U'); 
  big.writechar(2,4,'K');
  big.writechar(2,7,'F');
  big.writechar(2,10,'C');
  big.writechar(2,13,'E');
  big.writechar(2,16,'T');
  delay(3000);
  lcd.clear();
  lcd.print("BACH");
  big.writechar(0,0,'2');
  big.writechar(0,3,'0');
  big.writechar(0,6,'1');
  big.writechar(0,9,'9');
  big.writechar(0,14,'2'); 
  big.writechar(0,17,'3');
  scrollMessage(3, scrollingMessage, 250, totalColumns);
  lcd.clear();

  emon.voltage(35, vCalibration, 1.7); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration); // Current: input pin, calibration.
  pinMode(15,OUTPUT);
  pinMode(5,OUTPUT);

  timer.setInterval(5000L, myTimerEvent);

}
 
void loop() {
  Blynk.run();
  timer.run();
}