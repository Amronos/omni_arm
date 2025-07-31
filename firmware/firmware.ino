/*
  Commands for control over Serial/UART:
  - m <ID> <position>: Move servo to a raw position value (0-1023).
  - d <ID> <degrees>: Move servo to a degree value (0-300).
  - f <ID>: Get all feedback for a servo (Position, Speed, Load, Voltage, Temperature).
  - p <ID>: Get position of a servo.
  - s <ID>: Get speed of a servo.
  - l <ID>: Get load of a servo.
  - v <ID>: Get voltage of a servo.
  - t <ID>: Get temperature of a servo.

  Example: "d 1 90" will move servo with ID 1 to 90 degrees.
*/

// The UART used to control servos.
#define S_RXD 18
#define S_TXD 19

#include <SCServo.h>

SCSCL sc;

// Command parsing variables
char cmd;
char argv1[16];
char argv2[16];
long arg1;
long arg2;
int arg_index = 0;
int char_index = 0;

void setup() {
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  sc.pSerial = &Serial1;
  delay(1000);
}

void loop() {
  while (Serial.available() > 0) {
    char chr = Serial.read();

    if (chr == '\r') {
      if (arg_index == 1) argv1[char_index] = '\0';
      else if (arg_index == 2) argv2[char_index] = '\0';
      runCommand();
      resetCommand();
    } else if (chr == ' ') {
      if (arg_index == 0) arg_index = 1;
      else if (arg_index == 1) {
        argv1[char_index] = '\0';
        arg_index = 2;
        char_index = 0;
      }
    } else {
      if (arg_index == 0) {
        cmd = chr;
      } else if (arg_index == 1) {
        argv1[char_index] = chr;
        char_index++;
      } else if (arg_index == 2) {
        argv2[char_index] = chr;
        char_index++;
      }
    }
  }
}

void resetCommand() {
  cmd = '\0';
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg_index = 0;
  char_index = 0;
}

void runCommand() {
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);

  switch (cmd) {
    case 'm':
      moveServo(arg1, arg2);
      break;
    case 'd':
      moveServoDegrees(arg1, arg2);
      break;
    case 'f':
      getFeedback(arg1);
      break;
    case 'p':
      getPosition(arg1);
      break;
    case 's':
      getSpeed(arg1);
      break;
    case 'l':
      getLoad(arg1);
      break;
    case 'v':
      getVoltage(arg1);
      break;
    case 't':
      getTemperature(arg1);
      break;
    default:
      Serial.println("Invalid Command");
      break;
  }
}

void moveServo(int id, int position) {
  sc.WritePos(id, position, 0, 1500); // Using default speed
  Serial.println("OK");
}

void moveServoDegrees(int id, int degrees) {
  int position = map(degrees, 0, 300, 0, 1023); // Assuming 300-degree servo
  sc.WritePos(id, position, 0, 1500);
  Serial.println("OK");
}

void getFeedback(int id) {
  if (sc.FeedBack(id) != -1) {
    Serial.print(sc.ReadPos(-1));
    Serial.print(" ");
    Serial.print(sc.ReadSpeed(-1));
    Serial.print(" ");
    Serial.print(sc.ReadLoad(-1));
    Serial.print(" ");
    Serial.print(sc.ReadVoltage(-1));
    Serial.print(" ");
    Serial.println(sc.ReadTemper(-1));
  } else {
    Serial.println("Error");
  }
}

void getPosition(int id) {
  int pos = sc.ReadPos(id);
  if (pos != -1) {
    Serial.println(pos);
  } else {
    Serial.println("Error");
  }
}

void getSpeed(int id) {
  int speed = sc.ReadSpeed(id);
  if (speed != -1) {
    Serial.println(speed);
  } else {
    Serial.println("Error");
  }
}

void getLoad(int id) {
  int load = sc.ReadLoad(id);
  if (load != -1) {
    Serial.println(load);
  } else {
    Serial.println("Error");
  }
}

void getVoltage(int id) {
  int voltage = sc.ReadVoltage(id);
  if (voltage != -1) {
    Serial.println(voltage);
  } else {
    Serial.println("Error");
  }
}

void getTemperature(int id) {
  int temp = sc.ReadTemper(id);
  if (temp != -1) {
    Serial.println(temp);
  } else {
    Serial.println("Error");
  }
}
