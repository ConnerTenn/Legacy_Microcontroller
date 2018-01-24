
void setup() 
{
  pinMode(9, OUTPUT);
}

bool State = 0;

void loop() 
{
  digitalWrite(9, State);
  State = !State;
}
