#include <SoftwareSerial.h>

#define rxPin 13
#define txPin 14

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
  mySerial.println("AT+CWJAP=\"Spectre\",\"\"\r");
  delay(5000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPMUX=1\r");
  delay(3000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPSTART=4,\"UDP\",\"192.168.43.8\",12345\r");
  delay(3000);
  while (mySerial.available())
    Serial.write(mySerial.read());
  mySerial.println("AT+CIPSEND=4,12\r");
  delay(1000);
  
  mySerial.println("hello les pd\r");
 
  
  
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
}
