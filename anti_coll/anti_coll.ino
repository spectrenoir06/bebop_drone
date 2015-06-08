/*                        ____   ____      .__  .__  .__                      */
/*                        \   \ /   /____  |  | |  | |__|__ __  _____         */
/*                         \   v   /\__  \ |  | |  | |  |  |  \/     \        */
/*  anti_coll.ino           \     /  / __ \|  |_|  |_|  |  |  /  | |  \       */
/*                           \___/  (____  /____/____/__|____/|__|_|  /       */
/*  06/01/2015                           \/                         \/        */
/*                          _________                     __                  */
/*  anton.alliot@gmail.com /   _____/_____   ____   _____/  |________   ____  */
/*  antoine@doussaud.org   \_____  \\____ \_/ __ \_/ ___\   __\_  __ \_/ __ \ */
/*                         /        \  |_> >  ___/\  \___|  |  |  | \/\  ___/ */
/*                        /_______  /   __/ \___  >\___  >__|  |__|    \___  >*/
/*                                \/|__|        \/     \/                  \/ */

#define class_piloting	0x00
#define cmd_flatTrim	0x00
#define cmd_takeoff		0x01
#define cmd_pcmd		0x02
#define cmd_land		0x03
#define cmd_emergency	0x04
#define id_project		0x01

typedef struct	s_packet
{
	uint8_t		frame_type;
	uint8_t		buffer_id;
	uint8_t		sequence;
	uint32_t	size;
	uint8_t		id;
	uint8_t		class_type;
	uint16_t	cmd;
}				t_packet;

typedef struct	s_pcmd
{
	uint8_t		flag;
	int8_t		roll;
	int8_t		pitch;
	int8_t		yaw;
	int8_t		gaz;
	float		psi;
}				t_arg_pcmd;

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
	0,		// flag   Boolean flag to activate roll/pitch movement
	0,		// roll   Roll consign for the drone  [-100;100]
	-100,	// pitch  Pitch consign for the drone [-100;100]
	0,		// yaw    Yaw consign for the drone   [-100;100]
	0,		// gaz    Gaz consign for the drone   [-100;100]
	0,		// psi [NOT USED] - Magnetic north heading of the controlling device (deg) [-180;180]
};

int		front_trig = 5;
int		front_echo = 4;

int		right_trig = 1;
int		right_echo = 2;

int		left_trig = 19;
int		left_echo = 20;

long	lecture_echo;
long	cm;

int		front, left, right;

void	take_off() {
	Serial1.println("AT+CIPSEND=4,11\r");

	while (Serial1.find(">"));

	Serial1.write((byte*)&packet, sizeof(packet));
	//Serial1.write('\r');
	//Serial1.write('\n');

	while (!Serial1.find("K")); // wait

	delay(5000);
}

void	ultrason_setup() {
	pinMode(left_trig, OUTPUT);
	pinMode(right_trig, OUTPUT);

	digitalWrite(left_trig, LOW);
	digitalWrite(right_trig, LOW);

	pinMode(left_echo, INPUT);
	pinMode(right_echo, INPUT);
}

void	wifi_setup() {
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

	take_off();
}

void setup() {
	ultrason_setup();
	wifi_setup();

	packet.frame_type = 0x02;
	packet.buffer_id  = 0x0a;
	packet.size       = 20;
	packet.cmd        = cmd_pcmd;
}

long scanLeft()
{
	digitalWrite(left_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(left_trig, LOW);
	lecture_echo = pulseIn(left_echo, HIGH);
	return (lecture_echo / 58);
}

long scanRight()
{
	digitalWrite(right_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(right_trig, LOW);
	lecture_echo = pulseIn(right_echo, HIGH);
	return (lecture_echo / 58);
}

long scanFront()
{
	digitalWrite(front_trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(front_trig, LOW);
	lecture_echo = pulseIn(front_echo, HIGH);
	return (lecture_echo / 58);
}

void    send_pack() {
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
