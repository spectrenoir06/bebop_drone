
#define class_piloting  0x00
#define cmd_flatTrim    0x00
#define cmd_takeoff     0x01
#define cmd_pcmd        0x02
#define cmd_land        0x03
#define cmd_emergency   0x04

#define id_project       0x01

typedef struct s_packet
{
  uint8_t  frame_type;
  uint8_t  buffer_id;
  uint8_t  sequence;
  uint32_t size;
  uint8_t  id;
  uint8_t  class_type;
  uint16_t cmd;
}              t_packet;

typedef struct s_pcmd
{
  uint8_t  flag;
  int8_t  roll;
  int8_t  pitch;
  int8_t  yaw;
  int8_t  gaz;
  float   psi;
}              t_arg_pcmd;

t_packet packet = {
  0x03,
  0x0b,
  0x42,
  11,
  id_project,
  class_piloting,
  cmd_takeoff,
};

t_arg_pcmd packet_pcmd_arg = {
  0,     // flag   Boolean flag to activate roll/pitch movement
  0,     // roll   Roll consign for the drone  [-100;100]
  -100,  // pitch  Pitch consign for the drone [-100;100]
  0,     // yaw    Yaw consign for the drone   [-100;100]
  0,     // gaz    Gaz consign for the drone   [-100;100]
  0,     // psi [NOT USED] - Magnetic north heading of the controlling device (deg) [-180;180]
};

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  Serial1.begin(115200);

  Serial.println("Start");

  Serial1.println("AT+RST\r");

  while (Serial1.find("ready"));

//  Serial1.println("AT+CWJAP=\"BebopDrone-B030867\",\"\"\r");
  Serial1.println("AT+CWJAP=\"Spectre\",\"\"\r");

  while (Serial1.find("OK"));

  Serial1.println("AT+CIPMUX=1\r");

  while (Serial1.find("OK"));


//  Serial1.println("AT+CIPSTART=4,\"UDP\",\"192.168.42.1\",54321\r");
  Serial1.println("AT+CIPSTART=4,\"UDP\",\"192.168.43.14\",54321\r");

  while (Serial1.find("OK"));

  Serial1.println("AT+CIPSEND=4,11\r");
  while (Serial1.find(">"));
  Serial1.write((byte*)&packet, sizeof(packet));
  //Serial1.write('\r');
  //Serial1.write('\n');

  delay(5000);

  while (Serial1.available())
    Serial.write(Serial1.read());

  packet.frame_type = 0x02;
  packet.buffer_id  = 0x0a;
  packet.size       = 20;
  packet.cmd        = cmd_pcmd;
}

void loop() // run over and over
{
    for (int i = 0; i < 50; i++)
    {
      packet.sequence = i%50;

      Serial1.println("AT+CIPSEND=4,20\r");   // send on channel 4 20byte

      while (!Serial1.find(">"));             // wait

      Serial1.write((byte*)&packet, sizeof(packet));                   // write 11byte
      Serial1.write((byte*)&packet_pcmd_arg, sizeof(packet_pcmd_arg)); // write 9byte
      //Serial1.write('\r');
      //Serial1.write('\n');

      while (!Serial1.find("K")); // wait
    }
    while (42)
    {
    }

}
