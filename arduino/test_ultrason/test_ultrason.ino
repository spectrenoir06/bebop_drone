int front_trig = 5; 
int front_echo = 4;

int right_trig = 1; 
int right_echo = 2;

int left_trig = 19; 
int left_echo = 20; 

long lecture_echo; 
long cm;

int front, left, right;

void setup() 
{ 
  pinMode(left_trig, OUTPUT);
  pinMode(right_trig, OUTPUT);
  
  digitalWrite(left_trig, LOW);
  digitalWrite(right_trig, LOW); 
  
  pinMode(left_echo, INPUT); 
  pinMode(right_echo, INPUT); 
  
  Serial.begin(9600); 
}

long scanLeft()
{
  digitalWrite(left_trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(left_trig, LOW); 
  lecture_echo = pulseIn(left_echo, HIGH); 
  return ((lecture_echo / 58) > 3000 ? 3000 : lecture_echo / 58);
}

long scanRight()
{
  digitalWrite(right_trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(right_trig, LOW); 
  lecture_echo = pulseIn(right_echo, HIGH); 
  return ((lecture_echo / 58) > 3000 ? 3000 : lecture_echo / 58);
}

long scanFront()
{
  digitalWrite(front_trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(front_trig, LOW); 
  lecture_echo = pulseIn(front_echo, HIGH); 
  return ((lecture_echo / 58) > 3000 ? 3000 : lecture_echo / 58);
}


void loop() 
{
  front = scanFront();
  left = scanLeft();
  right = scanRight();
  
  /* --------- send to computer ----------- */
  Serial.print(front);
  Serial.print(" ");
  Serial.print(left);
  Serial.print(" ");
  Serial.println(right); 
  /* -------------------------------------- */
 
  //delay(90); 
}
