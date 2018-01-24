
typedef unsigned char BYTE;

int SendPin = 11;
int RecievePin = 10;
int LEDPin = 9;

bool PlayTone = false;
unsigned long t1;

bool Sense(int sendPin, int recievePin)
{
  static uint16_t Input = 0;
  bool result = 1;
  
  Input <<= 1;
  digitalWrite(sendPin, 1);
  for (int i = 0; i < 1; i++)
  {
    if (digitalRead(recievePin) && result)
    {
      result = 0;
    }
  }

  Input |= result;
  digitalWrite(sendPin, 0);
  return Input;// & 0x0F;
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

  cli();

  //set timer0 interrupt at 2kHz
  TCCR0A = 0;// set entire TCCR2A register to 0
  TCCR0B = 0;// same for TCCR2B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 15;// = (16*10^6) / (2^6*1525*1) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS00 bits for no prescaler
  TCCR0B |= (1 << CS00);   
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  sei();//allow interrupts
}

void loop() 
{
  //digitalWrite(LEDPin, 0);
  if (Sense(SendPin, RecievePin))
  {
    PlayTone = true;
    //Tone();
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
  else
  {
    PlayTone = false;
  }
}

ISR(TIMER0_COMPA_vect)
{//timer0 interrupt 1MHz
  static bool flip = 1;
  static int count = 0;
  if (count > 100)
  {
    flip = !flip;
    count = 0;
    if (PlayTone) { digitalWrite(LEDPin, flip); }
  }
  count++;
}


