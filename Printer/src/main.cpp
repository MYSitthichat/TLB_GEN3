#include <Arduino.h>

#define MtrBrake 0
#define ClockWise 1
#define CClockWise 2
#define GndBreak 3
#define CounterSenseThresHold 100

///// Motor 1 ///// Pull
const int inApin1 = 7;
const int inBpin1 = 8;
const int pwmpin1 = 5;
///// Motor 2 ///// Slab
const int inApin2 = 4;
const int inBpin2 = 9;
const int pwmpin2 = 6;


bool limit_logic = false;

const int limit = 13;

const int slabswitch = 18;

const int switchPin1 = 10;
// const int switchPin2 = 11;
unsigned int main_state = 0;

int interprete = 0;
String message = "";
int cmd_from_serial = 0;

int machine_state = 0;
bool start_func = false;

// TIMER
unsigned long pretime = 0;


void motor1off();
void motor1Go(uint8_t direct, uint8_t pwm);
void motor2off();
void motor2Go(uint8_t direct, uint8_t pwm);


void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.flush();
  pinMode(inApin1, OUTPUT);
  pinMode(inBpin1, OUTPUT);
  pinMode(pwmpin1, OUTPUT);
  pinMode(inApin2, OUTPUT);
  pinMode(inBpin2, OUTPUT);
  pinMode(pwmpin2, OUTPUT);
  pinMode(limit, INPUT_PULLUP);
  pinMode(slabswitch, INPUT_PULLUP);
  pinMode(switchPin1, INPUT_PULLUP);
  motor1off();
  motor2off();
  Serial.println("Ready");
}

void loop()
{
  if(Serial.available())
    {
      char inByte = Serial.read();
      if(inByte == '\n')
      {
        interprete = 1;
        cmd_from_serial = 1;
      }
      else
      {
        message = message + String(inByte);
      }
    }
  if(interprete == 1)
      {
        if(cmd_from_serial == 1)
        {
          switch (message[0])
          {
            case 'g':
            {
              Serial.println("GO");
              machine_state = 1;
              start_func = true;
              break;
            }
            case 'r':
            {
              Serial.println("reset");
              asm volatile ("jmp 0");
              break;
            }
            case 's':
            {
              Serial.println("stop");
              motor1off();
              motor2off();
              break;
            }
            default:
            {
              Serial.println("Invalid Command");
              machine_state = 0;
              start_func = false;
              break;
            }
          }
        }
        message = "";
        interprete = 0;
      }
  if(start_func == true)
    {
      switch(machine_state)
          {
            case 1:
              {
                motor2Go(CClockWise, 255);
                machine_state = 2;
                break;
              }
            case 2:
              {
                if(digitalRead(slabswitch) == 0)
                {
                  motor2off();
                  machine_state = 3;
                }
                break;
              }
            case 3:
            {
              motor2Go(ClockWise, 255);
              pretime = millis();
              machine_state = 4;
              break;
            }
            case 4:
            {
              if(millis() - pretime >= 1500)
              {
                motor2off();
                machine_state = 5;
              }
              break;
            }
            case 5:
            {
              motor1Go(CClockWise, 255);
              machine_state = 6;
              break;
            }
            case 6:
            {
              if(digitalRead(limit) == 1)
              {
                motor1off();
                machine_state = 7;
              }
              break;
            }
            case 7:
            {
              motor2Go(CClockWise, 255);
              machine_state = 8;
              break;
            }
            case 8:
            {
              if(digitalRead(slabswitch) == 0)
              {
                motor2off();
                machine_state = 9;
              }
              break;
            }
            case 9:
            {
              motor2Go(ClockWise, 255);
              pretime = millis();
              machine_state = 10;
              break;
            }
            case 10:
            {
              if(millis() - pretime >= 1000)
              {
                motor2off();
                pretime = millis();
                machine_state = 11;
              }
              break;
            }
            case 11:
            {
              if(millis() - pretime >= 3000)
              {
                motor1Go(ClockWise, 255);
                pretime = millis();
                machine_state = 12;
              }
              break;
            }
            case 12:
            {
              if(millis() - pretime >= 2000)
              {
                motor1off();
                machine_state = 13;
              }
              break;
            }
            case 13:
            {
              motor2Go(CClockWise, 255);
              machine_state = 14;
              break;
            }
            case 14:
            {
              if(digitalRead(slabswitch) == 0)
              {
                motor2off();
                machine_state = 15;
              }
              break;
            }
            case 15:
            {
              start_func = false; 
              break;
            }

            default:
              {
                break;
                start_func = false; 
              }
          }
    }

}

void motor1off()
{
  digitalWrite(inApin1, LOW);
  digitalWrite(inBpin1, LOW);
  analogWrite(pwmpin1, 0);
}

void motor1Go(uint8_t direct, uint8_t pwm)
{
  if (direct <= 3)
  {
    digitalWrite(inApin1, (direct <= 1) ? HIGH : LOW);
    digitalWrite(inBpin1, (direct == 0 || direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin1, pwm);
  }
}

void motor2off()
{
  digitalWrite(inApin2, LOW);
  digitalWrite(inBpin2, LOW);
  analogWrite(pwmpin2, 0);
}

void motor2Go(uint8_t direct, uint8_t pwm)
{
  if (direct <= 3)
  {
    digitalWrite(inApin2, (direct <= 1) ? HIGH : LOW);
    digitalWrite(inBpin2, (direct == 0 || direct == 2) ? HIGH : LOW);
    analogWrite(pwmpin2, pwm);
  }
}



