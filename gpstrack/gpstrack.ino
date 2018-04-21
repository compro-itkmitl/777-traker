/* โปรเจคนี้ได้ทำการเปลี่ยนวิธีการแก้ปัญหาเล็กน้อย โดยเปลี่ยนเป็นการใช้อุปกรณ์
GPS module ในการรับค่าพิกัด โดยเป็นมาตรฐาน NMEA โดยใช้ library TinyGPS.h ในการถอดรหัสข้อมูล
3G module ในการส่งพิกัดค่าขึ้น ฐานข้อมูล Sql โดยไปเรียกใช้ script php
โดยอุปกรณ์ทุกตัวติดต่อกันผ่านทาง library Software Serial

Hardware:
Arduino Uno R3
GPS Module
3G Module UC15-T

เอกสารที่ศึกษาขณะทำโปรเจค:
คู่มือ 3G Module UC15-T ภาษาไทย
3G Module UC15-T Arduino Development Guide

โดยโค้ดนี้เป็นการเขียนขึ้นเองส่วนหนึ่ง โดยพยายามศึกษาจากโค้ดจากทั้งชาวต่างชาติและชาวไทยบน internet
ประกอบกับการสอบถามผู้เชี่ยวชาญ
*/

#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS gps; // ref TinyGPS as gps
SoftwareSerial sGPS(10, 11); // define comunication with GPS module by 10, 11 legs call sGPS
SoftwareSerial ATDevice(8, 9); // define comunication with 3G module by 8, 9 legs call ATDevice

float lat, lon;                //define temporary latitude, longtitude variable
float plat, plon;              //define present latitude, longtitude variable
int new_data = 0;             //define switch variable new_data
void ATcom(const char* msg, int wait);     //define ATcommand via code function ประกาศฟังก์ชั่น การสั่งการ AT-Command โดยโค้ด
int digit(int n);              //define digit function which find amount of digits of n ประกาศฟังชั่นในการคำนวนจำนวนหลักของเลข

void setup() {
  Serial.begin(9600);                  //เริ่มการทำงานของ Serial ที่ 9600
  sGPS.begin(9600);                    //เริ่มการทำงานของ sGPS ที่ 9600
  ATDevice.begin(9600);                //เริ่มการทำงาน ATDevice ที่ 9600
  pinMode(LED_BUILTIN, OUTPUT);        //ตั้งค่า led buildin 
  digitalWrite(LED_BUILTIN, LOW);      //ปิด led
  ATcom("AT+QICSGP=1,1,\"internet\",\"\",\"\",1;", 3000); //ตั้งค่าใช้งาน AIS
  ATcom("AT+QFTPCFG=\"contextid\",1;", 500);          //contextid = 1
  ATcom("AT+QHTTPCFG=\"responseheader\",1;", 500);    //อนุญาตให้ใช้ HTTP header
  Serial.println("GPS START:");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  sGPS.listen();                          //สลับช่องสัญญาณมาเป็น sGPS
  digitalWrite(LED_BUILTIN, HIGH);        //เปิด LED เพื่อให้รู้ว่าเริ่มทำงาน
  new_data = 0;                           // ยังไม่มีข้อมูลใหม่
  unsigned long start = millis();
  while (millis()-start < 1000){          //ในระยะเวลา 1 วิ ตรวจหาข้อมูลใหม่ โดยอ่านข้อมูลจาก GPS
    while (sGPS.available()){
      char c = sGPS.read();
      if (gps.encode(c)) new_data = 1;
    }
  }
  if (new_data){
    unsigned long age;
    gps.f_get_position(&lat, &lon, &age);  //รับข้อมูลจาก gps บรรจุลงตัวแปรสามตัว
    if ((lat != plat) || (lon != plon)){
      plat = lat;
      plon = lon;
      Serial.print("LATITUDE:");
      Serial.print(plat, 8);
      Serial.print("\n");
      Serial.print("LONGTITUDE:");
      Serial.print(plon, 8);
      Serial.print("\n");
      ATDevice.listen();          //สลับช่องสัญญาณมาเป็น ATDevice เพื่อส่งข้อมูลขึ้น Sql
      sendsql();                  //ฟังก์ชั่นในการส่งข้อมูล
      delay(700);
      }
    }
}

void sendsql(){
  ATcom("AT+QIACT=1;", 3000);     //เปิดใช้งาน internet
  
  ATDevice.print("AT+QHTTPURL=");   //ตั้งความยาว URL โดยใช้ ฟังก์ชั่น digit คำนวนหาจำนวนหลักของค่าพิกัด
  ATDevice.print(56+digit(plat)+digit(plon));
  ATDevice.println(",80");
  unsigned long startTime = millis();
  while (millis() - startTime < 3000) {
    if (ATDevice.available()) {
      char c = ATDevice.read();
      Serial.write(c);
    }
  }
  
  ATDevice.print("http://777traker.tk/add_data.php?t="); //สร้าง url ในการเก็ตค่า โดยใช้ตัวแปร t และ h ของ url 
  ATDevice.print(plat, 8);
  ATDevice.print("&h=");
  ATDevice.println(plon, 8);
  startTime = millis();
  while (millis() - startTime < 2000) {
    if (ATDevice.available()) {
      char c = ATDevice.read();
      Serial.write(c);
    }
  }
  
  ATcom("AT+QHTTPGET=80;", 1500);     //ทำการ get ค่า ลงฐานข้อมูล
  ATcom("AT+QIDEACT=1", 500);         //ปิดการใช้ internet
  }

void ATcom(const char* msg, int wait){
  Serial.println(msg);         //แสดงผล คำสั่งเพื่อตรวจสอบ
  ATDevice.println(msg);       //นำคำสั่งไปฝากไว้บน ATDevice
  unsigned long startTime = millis(); //ให้ ATDevice ทำการอ่านคำสั่ง ATCOMMAND
  while (millis() - startTime < wait) {
    if (ATDevice.available()) {
      char c = ATDevice.read();
      Serial.write(c);
    }
  }
}


int digit(int n){
  int count = 0;
  while(n != 0){ //หาร10เรื่อยๆแล้ว เพิ่ม count 1 ตราบใดที่ n ยังไม่เป็น 0 return count เป็นจำนวนหลัก
    n /= 10;
    ++count;
  }
  return count;
}
