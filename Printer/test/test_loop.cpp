#include <Arduino.h>

#define MtrBrake 0
#define ClockWise 1
#define CClockWise 2
#define GndBreak 3
#define CounterSenseThresHold 100

///// Motor 1 /////
const int inApin1 = 7;
const int inBpin1 = 8;
const int pwmpin1 = 5;
///// Motor 2 /////
const int inApin2 = 4;
const int inBpin2 = 9;
const int pwmpin2 = 6;

const int statpin = 13;

bool limit_logic = false;

const int limit1 = 11;
const int limit2 = 12;
const int limit3 = 13;

const int microswitch = 18;

const int switchPin1 = 10;
// const int switchPin2 = 11;

unsigned int main_state = 0;

String inputString = "";

void motorOff1();
void motorGo1(uint8_t direct, uint8_t pwm);
void motorOff2();
void motorGo2(uint8_t direct, uint8_t pwm);

void setup()
{
  Serial.begin(9600);
  pinMode(statpin, OUTPUT);
  pinMode(inApin1, OUTPUT);
  pinMode(inBpin1, OUTPUT);
  pinMode(pwmpin1, OUTPUT);
  pinMode(inApin2, OUTPUT);
  pinMode(inBpin2, OUTPUT);
  pinMode(pwmpin2, OUTPUT);
  pinMode(limit1, INPUT_PULLUP);
  pinMode(limit2, INPUT_PULLUP);
  pinMode(limit3, INPUT_PULLUP);
  pinMode(microswitch, INPUT_PULLUP);
  pinMode(switchPin1, INPUT_PULLUP);

  motorOff1();
  motorOff2();
}

void loop()
{
  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      inputString.trim();
      if (inputString == "r1") {
        Serial.println("Command 'r1' received, starting state machine");
        main_state = 1;
      } else if (inputString == "r0") {
        Serial.println("Command 'r0' received, stopping all motors");
        motorOff1();
        motorOff2();
        main_state = 0; 
      } else if (inputString == "p1") {
        Serial.println("Command 'p1' received, running only part 1");
        main_state = 6;
      } else if (inputString == "p2") {
        Serial.println("Command 'p2' received, running only part 2");
        main_state = 7; 
      }
      inputString = "";
    }
  }

  int microswitchState = digitalRead(microswitch);
  limit_logic = digitalRead(limit1) && digitalRead(limit2) && digitalRead(limit3);

  switch (main_state)
  {
    case 0:
    {
      main_state = 1;
      break;
    }
      
    case 1:
      if (limit_logic == 0)
      {
        motorGo1(ClockWise, 255);
        Serial.println("state 0");
        main_state = 2;
        delay(3000);
      }
      break;
    case 2:
      if (limit_logic == 0)
      {
        motorGo1(CClockWise, 255);
        Serial.println("state 1");
        main_state = 3;
      }
      break;
    case 3:
      if (limit_logic == 1)
      {
        motorOff1();
        Serial.println("state 2");
        main_state = 4;
      }
      break;
    case 4:
      if (microswitchState == 1)
      {
        Serial.println("state 3");
        motorGo2(CClockWise, 255);
        delay(500);
        main_state = 5;
      }
      break;
    case 5:
      if (microswitchState == 0)
      {
        motorGo2(ClockWise, 255);
        Serial.println("state 4");
        delay(2200);
        motorOff2();
        Serial.println("state 4: motor off");
        main_state = 6;
      }
      break;
    case 6:
      if (limit_logic == 0)
      {
        motorGo1(ClockWise, 255);
        delay(2000);
        motorGo1(CClockWise, 255);
        delay(2000);
        motorOff1();
        Serial.println("Part 1 complete");
        main_state = 0;
      }
      break;
    case 7:
      if (microswitchState == 0)
      {
        motorGo2(CClockWise, 255);
        delay(500);
        if (microswitchState == 0)
        {
          motorGo2(ClockWise, 255);
          delay(1500);
          motorOff2();
          Serial.println("Part 2 complete");
          main_state = 0;
        }
      }
      break;
      // case 8:
      // if (limit_logic == 0)
      // {
      //   motorGo1(ClockWise, 255);
      //   delay(2000);
      //   motorGo1(CClockWise, 255);
      //   delay(2000);
      //   motorOff1();
      //   Serial.println("Part 1 complete");
      //   main_state = 0;
      // }
    default:
      motorOff1();
      motorOff2();
      main_state = 0;
      break;
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
    digitalWrite(inBpin2, (direct == 0 || direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin2, pwm);
  }
}
