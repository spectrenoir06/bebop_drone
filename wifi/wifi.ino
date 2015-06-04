#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 14

#define frame_type      0x02

#define buffer_id       0x0a

#define id_project      0x01
#define class_piloting  0x00
#define cmd_flatTrim    0x0000;
#define cmd_takeoff     0x0100;

#define cmd_taland      0x0300;
#define cmd_emergency   0x0400;

byte packet[13] = {
  0x03,
  buffer_id,
  0x42,
  0x14, 0x00, 0x00, 0x00,
  id_project,
  class_piloting,
  0x01,
  0x00,
  '\r',
};

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("AT+RST");
  
  delay(8000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CWJAP=\"BebopDrone-B030867\",\"\"\r");
  delay(5000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPMUX=1\r");
  delay(3000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPSTART=4,\"UDP\",\"192.168.42.1\",54321\r");
  delay(3000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPSEND=4,13\r");
  delay(1000);
  
  mySerial.write((uint8_t)0x03);
  mySerial.write((uint8_t)0x0b);
  mySerial.write((uint8_t)0x42);
  mySerial.write((uint8_t)0x0b);
  mySerial.write((uint8_t)0);
  mySerial.write((uint8_t)0);
  mySerial.write((uint8_t)0);
  mySerial.write((uint8_t)0x01);
  mySerial.write((uint8_t)0x00);
  mySerial.write((uint8_t)0x01);
  mySerial.write((uint8_t)0x00);
  mySerial.write('\r');
  mySerial.write('\n');
  
  
  
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
