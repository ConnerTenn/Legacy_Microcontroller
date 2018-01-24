
int Delay = 2000;
unsigned long t1;

void setup() 
{
  pinMode(8, OUTPUT);
}

void loop() 
{
  digitalWrite(8, 1);
  //delay(1);
  t1 = micros();
  while (micros() - t1 < Delay) {}
  digitalWrite(8, 0);
  //delay(1);
  t1 = micros();
  while (micros() - t1 < Delay) {}
}
