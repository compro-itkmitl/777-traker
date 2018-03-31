#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS gps;
SoftwareSerial sGPS(10, 11); // TX, RX

float lat, lon;
float plat, plon;
int new_data = 0;

void setup() {
  Serial.begin(9600);
  sGPS.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("GPS START:");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, HIGH);
  new_data = 0;
  unsigned long start = millis();
  while (millis()-start < 1000){
    while (sGPS.available()){
      char c = sGPS.read();
      if (gps.encode(c)) new_data = 1;
    }
  }
  if (new_data){
    unsigned long age;
    gps.f_get_position(&lat, &lon, &age);
    if ((lat != plat) || (lon != plon)){
      plat = lat;
      plon = lon;
      Serial.print("LATITUDE:");
      Serial.print(plat, 6);
      Serial.print("\n");
      Serial.print("LONGTITUDE:");
      Serial.print(plon, 6);
      Serial.print("\n");
      delay(1000);
      }
    }
}

