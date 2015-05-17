int trig = 1; 
int echo = 2; 
long lecture_echo; 
long cm;

void setup() 
{ 
  pinMode(trig, OUTPUT); 
  digitalWrite(trig, LOW); 
  pinMode(echo, INPUT); 
  Serial.begin(9600); 
}

void loop() 
{ 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  lecture_echo = pulseIn(echo, HIGH); 
  cm = lecture_echo / 58; 
  Serial.println(cm); 
  delay(100); 
}
