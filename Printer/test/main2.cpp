#include <Arduino.h>

#define MtrBrake 0
#define ClockWise 1
#define CClockWise 2
#define GndBreak 3
#define CounterSenseThresHold 100

// VNH2SP30 pin definitions
int inApin2 = 4;
int inBpin2 = 9;
int pwmpin2 = 6;

int statpin = 13;

// Pin definitions for switches
const int switchPin3 = 10; // สวิตช์ตัวที่ 3
const int switchPin4 = 11; // สวิตช์ตัวที่ 4

void motorOff2();
void motorGo2(uint8_t direct, uint8_t pwm);

void setup()
{
  Serial.begin(9600);
  pinMode(statpin, OUTPUT);

  pinMode(inApin2, OUTPUT);
  pinMode(inBpin2, OUTPUT);
  pinMode(pwmpin2, OUTPUT);

  pinMode(switchPin3, INPUT_PULLUP);
  pinMode(switchPin4, INPUT_PULLUP);

  motorOff2();
}

void loop()
{
  // อ่านสถานะสวิตช์
  bool switch3State = !digitalRead(switchPin3);
  bool switch4State = !digitalRead(switchPin4);

  // ควบคุมมอเตอร์ตามสถานะของสวิตช์
  if (switch3State && !switch4State)
  {
    motorGo2(ClockWise, 255); // เดินหน้า
  }
  else if (!switch3State && switch4State)
  {
    motorGo2(CClockWise, 255); // ถอยหลัง
  }
  else
  {
    motorOff2(); // หยุดมอเตอร์
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
    digitalWrite(inBpin2, (direct == 0 || direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin2, pwm);
  }
}
