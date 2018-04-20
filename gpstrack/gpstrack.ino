#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS gps;
SoftwareSerial sGPS(10, 11); // TX, RX
SoftwareSerial ATDevice(8, 9);

float lat, lon;
float plat, plon;
int new_data = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("GPS START:");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  sGPS.begin(9600);
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
      Serial.print(plat, 8);
      Serial.print("\n");
      Serial.print("LONGTITUDE:");
      Serial.print(plon, 8);
      Serial.print("\n");
      ATDevice.begin(9600);
      sendsql();
      delay(1000);
      }
    }
}

void sendsql(){
  Serial.print(command("AT+QICSGP=1,1,\"internet\",\"\",\"\",1;", 5000));
  Serial.print(command("AT+QFTPCFG=\"contextid\",1;", 5000));
  Serial.print(command("AT+QHTTPCFG=\"responseheader\",1;", 5000));
  Serial.print(command("AT+QIACT=1;", 5000));
  Serial.print(command("AT+QHTTPURL=61,80;", 5000));
  Serial.print(command("http://777traker.tk/add_data.php?t=12.11116666&h=123.11114444;", 5000));
  Serial.print(command("AT+QHTTPGET=80;", 5000));
  Serial.print(command("AT+QHTTPREAD=80;", 5000));
  Serial.print(command("AT+QIDEACT=1", 5000));
}

String command(const char *toSend, unsigned long milliseconds) {
  String result;
  Serial.print("Sending: ");
  Serial.println(toSend);
  ATDevice.println(toSend);
  unsigned long startTime = millis();
  Serial.print("Received: ");
  while (millis() - startTime < milliseconds) {
    if (ATDevice.available()) {
      char c = ATDevice.read();
      Serial.write(c);
      result += c;  // append to the result string
    }
  }
Serial.println();  // new line after timeout.
return result;
}

int digit(int n){
  int count = 0;
  while(n != 0){
    n /= 10;
    ++count;
  }
  return count;
}
