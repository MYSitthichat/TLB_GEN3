#include <Arduino.h>
const int detect_tube = 2;
const int detect_sticker1 = 4;
const int detect_sticker2 = 5; 
const int solinoid_re1 = 10;
const int solinoid_re2 = 11;
const int rolling = 9;

int tube = 0;
int sticker = 0;
int sticker2 = 0;

int interprete = 0;
int cmd_from_serial = 0;
String message = "";

bool start_flag = false;
int machine_state = 0;
bool start_rolling_race1 = false;

void stoped(void);
void stop_rolling(void);

void setup() 
  {
    Serial.begin(115200);
    Serial.flush();
    pinMode(detect_tube, INPUT);
    pinMode(detect_sticker1, INPUT);
    pinMode(detect_sticker2, INPUT);
    pinMode(solinoid_re1, OUTPUT);
    pinMode(solinoid_re2, OUTPUT);
    pinMode(rolling, OUTPUT);
  }

void loop() 
  {
    tube = digitalRead(detect_tube);
    sticker = digitalRead(detect_sticker1);
    sticker2 = digitalRead(detect_sticker2);

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
        switch(message[0])
        {
          case 'g':
            {
              Serial.println("start");
              start_flag = true;
              machine_state = 1;
              message = "";
              interprete = 0;
              break;
            }
          case 's':
            {
              Serial.println("Stop");
              start_flag = false;
              machine_state = 0;
              stoped();
              message = "";
              interprete = 0;
              break;
            }  
          default:
            {
              Serial.println("Invalid Command");
              message = "";
              interprete = 0;
              break;
            }
        }

      }
    if(tube == 0)
      {
        start_flag = true;
        machine_state = 1;
      }

    if(start_flag == true)
      {
        switch(machine_state)
        {
          case 1:
            {
              start_rolling_race1 = true;
              machine_state = 2;
              break;
            }
            case 2:
            {
              // Serial.print(sticker);
              // Serial.print("=======");
              // Serial.println(sticker2);
              break;
            }

          default:
            {
              break;
            }

        }
      }
    if(start_rolling_race1 == true)
      {
        digitalWrite(rolling,HIGH);
        delayMicroseconds(200);
        digitalWrite(rolling,LOW);
        delayMicroseconds(200);
      }

  }
void stop_rolling(void)
{
  digitalWrite(rolling,LOW);
  Serial.println("Rolling Stoped");
}
void stoped(void)
{
  digitalWrite(rolling, LOW);
  digitalWrite(solinoid_re1, LOW);
  digitalWrite(solinoid_re2, LOW);
  stop_rolling();
  start_flag = false;
  start_rolling_race1 = false;
  machine_state = 0;
  Serial.println("Machine Stoped");
}
