
int SendPin = 11;
int RecievePin = 10;
int LEDPin = 9;

bool Press = 0;

typedef unsigned char BYTE;
BYTE Input = 0;

bool Sense(int sendPin, int recievePin)
{
  bool result = 1;
  //unsigned int count = 0;
  /*for (int i = 0; i < 30 && !result; i++)
  {
    count = 0;
    digitalWrite(sendPin, 1);
    while (digitalRead(recievePin) != 1 && !result)
    {
      count++;
      if (count > 3) { result = 1; }
    }
    count = 0;
    digitalWrite(sendPin, 0);
    while (digitalRead(recievePin) != 0 && !result)
    {
      count++;
      if (count > 3) { result = 1; }
    }
  }*/
  Input <<= 1;
  digitalWrite(sendPin, 1);
  for (int i = 0; i < 10; i++)
  {
    if (digitalRead(recievePin) && result)
    {
      //count = i;
      result = 0;
    }
  }
  //if (count > 10)
  //{
    //Input = Input | 1;
    //Input |= 1;
  //}
  Input |= result;
  digitalWrite(sendPin, 0);
  return Input & 0x0F;
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
  Press = Sense(SendPin, RecievePin);
  //Input *= 2; //Input = Input & 0x7E;
  //Input += digitalRead(RecievePin);
  //Press = (bool)Input;
  //Press = digitalRead(RecievePin);
  
  digitalWrite(LEDPin, Press);
  Serial.print(Press);
  Serial.print("    ");
  Serial.print(Input & 0xF);
  Serial.println();
}
