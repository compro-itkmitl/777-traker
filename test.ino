#include <SoftwareSerial.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(D3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D3, LOW);
  delay(50);
  digitalWrite(D3, HIGH); 
  delay(50);
}
