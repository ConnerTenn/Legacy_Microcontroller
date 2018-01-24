#include <Servo.h> 

//Servo Out;

volatile unsigned long ANALOG[2] = {49, 52};
volatile unsigned long DIGITAL[2] = {51, 50};

volatile unsigned long DigitalStartTime[2];
volatile unsigned long AnalogStartTime[2];
volatile int DigitalPulseTime[2]; 
volatile int AnalogPulseTime[2];

Servo Out[4][2];
//this is the time that the last interrupt occurred. 
//you can use this to determine if your receiver has a signal or not.
//volatile int last_interrupt_time;


void SwitchInput0()
{
  int Input = 0;
  //last_interrupt_time = micros();
  if(digitalRead(DIGITAL[Input]) == HIGH)
  {
    DigitalStartTime[Input] = micros();
  }
  else
  {
    if(DigitalStartTime[Input] != 0)
    {
      DigitalPulseTime[Input]= ((volatile int)micros() - DigitalStartTime[Input]);
      DigitalStartTime[Input] = 0;
    }
  }
}

void SwitchInput1()
{
  int Input = 1;
  //last_interrupt_time = micros();
  if(digitalRead(DIGITAL[Input]) == HIGH)
  {
    DigitalStartTime[Input] = micros();
  }
  else
  {
    if(DigitalStartTime[Input] != 0)
    {
      DigitalPulseTime[Input]= ((volatile int)micros() - DigitalStartTime[Input]);
      DigitalStartTime[Input] = 0;
    }
  }
}

void StickInput0()
{
  int Input = 0;
  //last_interrupt_time = micros();
  if(digitalRead(ANALOG[Input]) == HIGH)
  {
    AnalogStartTime[Input] = micros();
  }
  else
  {
    if(AnalogStartTime[Input] != 0)
    {
      AnalogPulseTime[Input] = ((volatile int)micros() - AnalogStartTime[Input]);
      AnalogStartTime[Input] = 0;
    }
  }
}
void StickInput1()
{
  int Input = 1;
  //last_interrupt_time = micros();
  if(digitalRead(ANALOG[Input]) == HIGH)
  {
    AnalogStartTime[Input] = micros();
  }
  else
  {
    if(AnalogStartTime[Input] != 0)
    {
      AnalogPulseTime[Input] = ((volatile int)micros() - AnalogStartTime[Input]);
      AnalogStartTime[Input] = 0;
    }
  }
}



void setup()
{
  {/*Setup Outputs*/}
  {
    int Pin = 23;
    for(int i = 0; i < 4; i++)
    {
      for(int q = 0; q < 2; q++)
      {
        Out[i][q].attach(Pin);
        Pin += 2;
      }
    }
  }//End Setup Inputs
  
  attachInterrupt(DIGITAL[0], SwitchInput0, CHANGE);
  attachInterrupt(DIGITAL[1], SwitchInput1, CHANGE);
  
  attachInterrupt(ANALOG[0], StickInput0, CHANGE);
  attachInterrupt(ANALOG[1], StickInput1, CHANGE);
  
  Serial.begin(9600);
  //Out.attach(52);
}

void loop()
{
  Serial.print(AnalogPulseTime[0]);
  Serial.print("  ");
  Serial.print(AnalogPulseTime[1]);
  Serial.print("  ");
  Serial.print(DigitalPulseTime[0]);
  Serial.print("  ");
  Serial.print(DigitalPulseTime[1]);
  Serial.print("  ");
  
  if (DigitalPulseTime[0] < 1400 && DigitalPulseTime[1] < 1500 )
  {
    Serial.print("1");
    Serial.print("   ");
    Serial.println(AnalogPulseTime[0]);
    Out[0][0].writeMicroseconds(AnalogPulseTime[0]);
    Out[0][1].writeMicroseconds(AnalogPulseTime[1]);
    
    Out[1][0].writeMicroseconds(1500);
    Out[1][1].writeMicroseconds(1500);
    Out[2][0].writeMicroseconds(1500);
    Out[2][1].writeMicroseconds(1500);
    Out[3][0].writeMicroseconds(1500);
    Out[3][1].writeMicroseconds(1500);
  }
  else if (DigitalPulseTime[0] > 1400 && DigitalPulseTime[1] < 1500 )
  {
    Serial.println("2");
    Out[1][0].writeMicroseconds(AnalogPulseTime[0]);
    Out[1][1].writeMicroseconds(AnalogPulseTime[1]);
    
    Out[0][0].writeMicroseconds(1500);
    Out[0][1].writeMicroseconds(1500);
    Out[2][0].writeMicroseconds(1500);
    Out[2][1].writeMicroseconds(1500);
    Out[3][0].writeMicroseconds(1500);
    Out[3][1].writeMicroseconds(1500);
  }
  else if (DigitalPulseTime[0] < 1400 && DigitalPulseTime[1] > 1500 )
  {
    Serial.println("3");
    Out[2][0].writeMicroseconds(AnalogPulseTime[0]);
    Out[2][1].writeMicroseconds(AnalogPulseTime[1]);
    
    Out[0][0].writeMicroseconds(1500);
    Out[0][1].writeMicroseconds(1500);
    Out[1][0].writeMicroseconds(1500);
    Out[1][1].writeMicroseconds(1500);
    Out[3][0].writeMicroseconds(1500);
    Out[3][1].writeMicroseconds(1500);
  }
  else if (DigitalPulseTime[0] > 1400 && DigitalPulseTime[1] > 1500 )
  {
    Serial.println("4");
    Out[3][0].writeMicroseconds(AnalogPulseTime[0]);
    Out[3][1].writeMicroseconds(AnalogPulseTime[1]);
    
    Out[0][0].writeMicroseconds(1500);
    Out[0][1].writeMicroseconds(1500);
    Out[1][0].writeMicroseconds(1500);
    Out[1][1].writeMicroseconds(1500);
    Out[2][0].writeMicroseconds(1500);
    Out[2][1].writeMicroseconds(1500);
  }
}





