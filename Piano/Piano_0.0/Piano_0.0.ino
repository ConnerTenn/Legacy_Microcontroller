
typedef unsigned char BYTE;

int SendPin = 11;
int RecievePin = 10;
int LEDPin = 9;

int Delay = 2000;//2000;
unsigned long t1;

bool Sense(int sendPin, int recievePin)
{
  static BYTE Input = 0;
  bool result = 1;
  
  Input <<= 1;
  digitalWrite(sendPin, 1);
  for (int i = 0; i < 10; i++)
  {
    if (digitalRead(recievePin) && result)
    {
      result = 0;
    }
  }

  Input |= result;
  digitalWrite(sendPin, 0);
  return Input & 0x0F;
}

void Tone()
{
  static double Pos = 0;
  static double Value = 0;
  Value += 0.1; 
  Pos = pow(10.0, fmod(Value, 2.0));
  //analogWrite(LEDPin, Pos * 255);
  //analogWrite(LEDPin, 255/2);
  
  digitalWrite(LEDPin, 1);
  digitalWrite(8, 1);
  //delay(1);
  t1 = micros();
  while (micros() - t1 < Pos * 100.0) {}
  digitalWrite(8, 0);
  //delay(1);
  t1 = micros();
  while (micros() - t1 < Pos * 100.0) {}
}

void setup() 
{
  pinMode(SendPin, OUTPUT); digitalWrite(SendPin, LOW);
  pinMode(RecievePin, INPUT);
  pinMode(LEDPin, OUTPUT); digitalWrite(LEDPin, LOW);

  Serial.begin(9600);
}

void loop() 
{
  digitalWrite(LEDPin, 0);
  if (Sense(SendPin, RecievePin))
  {
    Tone();
    /*digitalWrite(LEDPin, 1);
    digitalWrite(8, 1);
    //delay(1);
    t1 = micros();
    while (micros() - t1 < Delay) {}
    digitalWrite(8, 0);
    //delay(1);
    t1 = micros();
    while (micros() - t1 < Delay) {}*/
  }
}


