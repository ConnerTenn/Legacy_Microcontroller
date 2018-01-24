
#include <Servo.h> 
//#include <Scheduler.h>
 
Servo Stick[2];
Servo Switch[2];

int StickIn[2];
int SwitchIn[2];

Servo Out[4][2];

int pos = 0;
 
void setup() 
{ 
  
  Serial.begin(115200);
  {/*Setup Inputs*/}
  {
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    pinMode(6, INPUT);
    pinMode(7, INPUT);
  }
  
  {/*Setup Outputs*/}
  {
    int Pin = 31;
    for(int i = 0; i < 4; i++)
    {
      for(int q = 0; q < 2; q++)
      {
        Out[i][q].attach(Pin);
        Pin += 2;
      }
    }
  }//End Setup Inputs
} 
 
void loop() 
{
  //StickIn[0] = Stick[0].read();
  //StickIn[1] = Stick[1].read();
  StickIn[0]+=5;
  //StickIn[0] = pulseIn(4, HIGH) - 200;
  StickIn[1] = pulseIn(5, HIGH) - 200;
  
  //SwitchIn[0] = pulseIn(6, HIGH) - 200;
  //SwitchIn[1] = pulseIn(7, HIGH) - 200;
  Serial.print(StickIn[0]);
  Serial.print("  ");
  Serial.println(StickIn[1]);
  Serial.println();
  
  Out[0][0].writeMicroseconds(StickIn[0]);
  //SwitchIn[0] = Switch[0].read();
  //SwitchIn[1] = Switch[1].read();
  /*
  if (SwitchIn[0] < 90 && SwitchIn[1] < 90)
  {
    Out[0][0].writeMicroseconds(StickIn[0]);
    Out[0][1].writeMicroseconds(StickIn[1]);
  }
  else if (SwitchIn[0] > 90 && SwitchIn[1] < 90)
  {
    Out[1][0].write(StickIn[0]);
    Out[1][1].write(StickIn[1]);
  }
  else if (SwitchIn[0] < 90 && SwitchIn[1] > 90)
  {
    Out[2][0].write(StickIn[0]);
    Out[2][1].write(StickIn[1]);
  }
  else if (SwitchIn[0] > 90 && SwitchIn[1] > 90)
  {
    Out[3][0].write(StickIn[0]);
    Out[3][1].write(StickIn[1]);
  }*/
}

