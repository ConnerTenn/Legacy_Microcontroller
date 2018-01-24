#include <Servo.h>

int val;

Servo Out;
void setup() 
{
  Serial.begin(115200);
  Out.attach(3);
}

void loop() 
{
}

void serialEvent()
{
  val = atoi(Serial.readString().c_str());
  Serial.println(val);
  if (val != 0)
  {
    Out.write(val);//1-180
  }
}
