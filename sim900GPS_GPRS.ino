#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <Wire.h>
#include <DS1307new.h>

#define pinpowerSIM900 9

TinyGPS gps;
SoftwareSerial sGPS(2, 3);

uintl6_t startAddr = 0x000;
uintl6_t lastAddr;
uintl6_t TimeIsSet = 0xaa55;

int currentDay = 0;
int currentMonth = 0;
int currentYear = 0;
int currentMinute = 0;

float flat, flon;
float previousFlat = 0.0;
float previousFlon = 0.0;

void setup() {
  pinMode(pinPowerSIM900, OUTPUT) digitalWrite(pinPowerSIM900,LOW);
  
  powerUpOrDown();

  Serial.begin(9600);
  sGPS.begin(9600);

}

void loop() 
{
  RTC.getTime();
currentDay = RTC.day;
currentMonth = RTC.month;
currentYear = RTC.year;
currentHour = RTC.hour;
currentMinute = RTC.minute;
}

void powerUpOrDown()
{
 digitalWrite(pinPowerSIM900,HIGH); 
 delay(2000);
 digitalWrite(pinPowerSIM900,LOW);
 delay(3000);
  
}

bool newData = false;
for (unsigned long start = millis(); millis() - start < 1000;)
{
 while (sGPS.available())
 {
  char c = sGPS.read();
  if (gps.encode(c))
  newData = true;
  } 
 }

 if (newData)
 {
  unsigned long age;
  gps.f_get_position(&flat, &flon, &age);

  if((flat != previousFlat) || (flon != previousFlon))
  {
    previousFLat = flat;
    previousFLon = flon;
    sendSQL();
  }
 }

 void SendSQL()
 {
  Serial.println("AT+SAPBR=3,1,\"comtype\",\"GPRS\"");
  delay(500);
  Serial.println("AT+SAPBR=3,1,\"APN\",\"internet\"");
  delay(500);
  Serial.println("AT+SAPBR=1,1");
  delay(500);
  Serial.println("AT+SAPBR=2,1");
  delay(3000);
  Serial.println("AT+HTTPINIT");
  delay(500);
  Serial.println("AT+HTTPPARA=\"CID\",1");
  delay(500);
  Serial.print("AT+HTTPPARA=\"URL\",\"http://777traker.tk/add_data,php")
  Serial.print("addDay=");
  if (currentDay < 10) {Serial.print("0"); Serial.print(currentDay);}
  else {Serial.print(currentDay);}
  Serial.print("/");
  if (currentMonth < 10) {Serial.print("0"); Serial.print(currentMonth);}
  else {Serial.print(currentMonth);}
  Serial.print("/");
  Serial.print(currentYear);
  Serial.print("&addHour=");
  if (currentHour < 10) {Serial.print("0"); Serial.print(currentHour);}
  else {Serial.print(currentMonth);}
  Serial.print(":");
   if (currentMinute < 10) {Serial.print("0"); Serial.print(currentMinute);}
  else {Serial.print(currentMinute);}
  Serial.print("&addLati=");
  Serial.print(previousFLat, 6);
  Serial.print("&addLongti=");
  Serial.print(previousFLon, 6);
  Serial.println("\"");
  delay(3000);
  Serial.println("AT+HTTPACTION=0");
  delay(15000);
  Serial.println("AT+HTTPTERM");
  delay(500);
  Serial.println("AT+SAPBR=0,1");
  delay(500);
  
 }
