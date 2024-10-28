#include <SoftwareSerial.h>
///////รับค่าจาก Raspberry Pi Pico ผ่าน RS485///////

SoftwareSerial RS485(2, 3); // RX, TX
String receivedCommand = "";
#define MtrBrake 0
#define ClockWise 1
#define CClockWise 2
#define GndBreak 3
#define CounterSenseThresHold 100



/////motor1//////
int inApin1 = 7;
int inBpin1 = 8;
int pwmpin1 = 5;
//////motor2//////
int inApin2 = 4;
int inBpin2 = 9;
int pwmpin2 = 6;

unsigned int main_state = 0;

void motorOff1();
void motorGo1(uint8_t direct, uint8_t pwm);
void motorOff2();
void motorGo2(uint8_t direct, uint8_t pwm);

 void setup() {
     Serial.begin(9600);
     RS485.begin(9600);
 }

 void loop() {
     if (RS485.available()) {
         char c = RS485.read();
         if (c == '\n') {
             processCommand(receivedCommand);
             receivedCommand = "";
         } else {
             receivedCommand += c;
         }
     }
 }

 void processCommand(String command) {
    switch (main_state)
  {
    case 0:
      if ( command == "START")
      {
        motorGo1(ClockWise, 255);
        delay(2000);
        Serial.println("state 0");
        main_state = 1;
      }
      break;
    default:
      motorOff1();
      motorOff2();
      main_state = 0;
      break;
  }
     if (command == "START") {
         // เริ่มมอเตอร์
     } else if (command == "STOP") {
         // หยุดมอเตอร์
     } else if (command.startsWith("SPEED:")) {
         int speed = command.substring(6).toInt();
         // ปรับความเร็วมอเตอร์ (speed)
     }
 }



 void motorOff1()
{
  digitalWrite(inApin1, LOW);
  digitalWrite(inBpin1, LOW);
  analogWrite(pwmpin1, 0);
}

void motorGo1(uint8_t direct, uint8_t pwm)
{
  if (direct <= 3)
  {
    digitalWrite(inApin1, (direct <= 1) ? HIGH : LOW);
    digitalWrite(inBpin1, (direct == 0 || direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin1, pwm);
  }
}

void motorOff2()
{
  digitalWrite(inApin2, LOW);
  digitalWrite(inBpin2, LOW);
  analogWrite(pwmpin2, 0);
}

void motorGo2(uint8_t direct, uint8_t pwm)
{
  if (direct <= 3)
  {
    digitalWrite(inApin2, (direct <= 1) ? HIGH : LOW);
    digitalWrite(inBpin2, (direct == 0 or direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin2, pwm);
  }
}

