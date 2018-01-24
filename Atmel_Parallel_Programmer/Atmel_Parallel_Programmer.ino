/*  
 HVFuse - Use High Voltage Programming Mode to Set Fuses on ATmega48/88/168
 09/23/08  Jeff Keyzer  http://mightyohm.com                
 The HV programming routines are based on those described in the
 ATmega48/88/168 datasheet 2545M-AVR-09/07, pg. 290-297
 This program should work for other members of the AVR family, but has only
 been verified to work with the ATmega168.  If it works for you, please
 let me know!  http://mightyohm.com/blog/contact/


 This program has been modified to be compatible with the AT90s2313 and AT90s8515
 */

#include <SoftwareSerial.h>

// Default fuse configuration
#define  FUSES  B11011111 //B11011110 //SPIEN_FUSE:5 FSTRT_FUSE:0

// Default lock bit configuration
#define LOCK_BITS B11111111 //B11111100

// Pin Assignments
#define  DATAD DDRD    //                             //     Data direction register for DATA port
#define  PIN PIND      //                             //     Input DATA port

//                AtMega328 Pins  2313      8515      comments
#define  DATA  PORTD   //PD       PB        PB        //     PORTD = Arduino Digital pins 0-7

#define VCC    8       //14       20        40        
#define RDY    12      //18       3         11        //    RDY/!BSY signal from target
#define OE     11      //17       6         12        
#define WR     10      //16       7         13         
#define BS     9       //15       8         14        
#define XA0    13      //19       9         15        
#define XA1    18      //27       11        16        //     Analog inputs 0-5 can be addressed as
#define RST    14      //23       1         9         //     Output to level shifter for !RESET
#define XTAL1  17      //26       5         19        

#define BUTTON 15      //24                           // Run button

#define DISP0  19      //28                           //Display results
#define DISP1  16      //25                           //Display results
#define RX     19      //28
#define TX     16      //25

SoftwareSerial SoftSerial(RX, TX);

void setup()  // run once, when the sketch starts
{
  SoftSerial.begin(9600);
  
  // Set up control lines for HV parallel programming
  DATA = 0x00;  // Clear digital pins 0-7
  DATAD = 0xFF; // set digital pins 0-7 as outputs
  pinMode(VCC, OUTPUT);
  pinMode(RDY, INPUT);
  pinMode(OE, OUTPUT);
  pinMode(WR, OUTPUT);
  pinMode(BS, OUTPUT);
  pinMode(XA0, OUTPUT);
  pinMode(XA1, OUTPUT);
  pinMode(RST, OUTPUT);  // signal to level shifter for +12V !RESET
  pinMode(XTAL1, OUTPUT);

  pinMode(DISP0, OUTPUT);
  pinMode(DISP1, OUTPUT);

  pinMode(BUTTON, INPUT);
  digitalWrite(BUTTON, HIGH);  // turn on pullup resistor

  // Initialize output pins as needed
  digitalWrite(RST, HIGH);  // Level shifter is inverting, this shuts off 12V
}

void loop()  // run over and over again
{
  while(digitalRead(BUTTON) == HIGH) {}  // wait until button is pressed
  
  // Initialize pins to enter programming mode
  //digitalWrite(PAGEL, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS, LOW);
  // Enter programming mode
  digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
  digitalWrite(WR, HIGH);  // Now we can assert !OE and !WR
  digitalWrite(OE, HIGH);
  delayMicroseconds(200);
  digitalWrite(RST, LOW);   // Apply 12V to !RESET thru level shifter
  delayMicroseconds(1000);
  // Now we're in programming mode until RST is set HIGH again

  //read current settings
  SoftSerial.println("Previous:");
  ReadFuseAndLockBitsSerial();
  
  ChipErase();
  
  // Program fuses
  WriteFuseBits(FUSES);

  //Program lock bits
  WriteLockBits(LOCK_BITS);
  
  //verify changes
  SoftSerial.println("New:");
  ReadFuseAndLockBitsSerial();
  
  delay(1000);  // wait a while to allow button to be released

  // Exit programming mode
  digitalWrite(RST, HIGH);

  // Turn off all outputs
  DATA = 0x00;
  digitalWrite(OE, LOW);
  digitalWrite(WR, LOW);
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, LOW);
  digitalWrite(BS, LOW);
  digitalWrite(VCC, LOW);
}




void SendCmd(byte command)  // Send command to target AVR
{
  // Set controls for command mode
  digitalWrite(XA1, HIGH);
  digitalWrite(XA0, LOW);
  digitalWrite(BS, LOW);
  //DATA = B01000000;  // Command to load fuse bits
  DATA = command;
  digitalWrite(XTAL1, HIGH);  // pulse XTAL to send command to target
  delayMicroseconds(1000);
  digitalWrite(XTAL1, LOW);
  delayMicroseconds(1000);
}

void ChipErase()
{
  SendCmd(B10000000);//load erase command
  
  //do write
  digitalWrite(WR, LOW);
  delay(10);
  digitalWrite(WR, HIGH);
  
  delayMicroseconds(1000);
}

void LoadDataLowByte(byte data)
{
  //Enable Data Loading
  digitalWrite(XA1, LOW);
  digitalWrite(XA0, HIGH);

  //select low byte
  //digitalWrite(BS, LOW);

  //set the data
  DATA = data;

  //pulse clock
  digitalWrite(XTAL1, HIGH);
  delayMicroseconds(1000);
  digitalWrite(XTAL1, LOW);
  delayMicroseconds(1000);
}

void WriteDataLowByte()
{
  //select low byte
  digitalWrite(BS, LOW);

  //begin write
  digitalWrite(WR, LOW);
  delayMicroseconds(300);
  digitalWrite(WR, HIGH);
  delayMicroseconds(300);
  while(!digitalRead(RDY)) {}
  
  delayMicroseconds(1000);
}

void WriteFuseBits(byte data)  // write high or low fuse to AVR
{
  SendCmd(B01000000); // Send command to enable fuse programming mode 
  LoadDataLowByte(data); //load fuse data

  //do write
  digitalWrite(WR, LOW);
  delayMicroseconds(1500);
  digitalWrite(WR, HIGH);
  
  delayMicroseconds(1000);
}

void WriteLockBits(byte data)
{
  SendCmd(B00100000);  // Send command to enable lock bit programming mode
  LoadDataLowByte(data); //load the bits
  WriteDataLowByte(); //write the bits
}

void ReadFuseAndLockBitsSerial()
{
  SendCmd(B00000100); //Read Fuse and Lock Bits
  
  //enable output
  digitalWrite(OE, LOW);
  digitalWrite(BS, HIGH);

  DATA = 0x00;
  DATAD = 0x00; // set digital pins 0-7 as inputs

  //SoftSerial.begin(9600);
    
  SoftSerial.print("Bus:");
  SoftSerial.println(PIN, BIN);
  //SPIEN: B00100000
  //FSTRT: B00000001
  //Lock1: B10000000
  //Lock2: B01000000
  
  //SoftSerial.print("SPIEN:");
  //SoftSerial.println((~PIN) & B00100000 == B00100000 ? 1 : 0);
  
  //SoftSerial.print("FSTRT:");
  //SoftSerial.println((~PIN) & B00000001 == B00000001 ? 1 : 0);
  
  //SoftSerial.print("Lock1:");
  //SoftSerial.println((~PIN) & B10000000 == B10000000 ? 1 : 0);
  
  //SoftSerial.print("Lock2:");
  //SoftSerial.println(~(PIN & ~B01000000), BIN);
  //SoftSerial.println(PIN), BIN);
  SoftSerial.println("");
  
  //SoftSerial.end();
  
  //dissable output
  digitalWrite(OE, HIGH);

  DATAD = 0xFF; // set digital pins 0-7 as outputs
}

/*
void ReadFuseAndLockBits()
{
  SendCmd(B00000100); //Read Fuse and Lock Bits
  
  //enable output
  digitalWrite(OE, LOW);
  digitalWrite(BS, HIGH);

  DATAD = 0x00; // set digital pins 0-7 as inputs
  
  //Serial.print("SPIEN:"); Serial.print(DATA & B00100000 == B00100000);
  //Serial.print("FSTRT:"); Serial.print(DATA & B00000001 == B00000001);
  //Serial.print("Lock1:"); Serial.print(DATA & B10000000 == B10000000);
  //Serial.print("Lock2:"); Serial.print(DATA & B01000000 == B01000000);

  //SPIEN
  digitalWrite(DISP0, HIGH);
  digitalWrite(DISP1, DATA & B00100000);
  delay(1000);
  digitalWrite(DISP0, LOW);
  digitalWrite(DISP1, LOW);
  delay(1000);

  //FSTRT
  digitalWrite(DISP0, HIGH);
  digitalWrite(DISP1, DATA & B00000001);
  delay(1000);
  digitalWrite(DISP0, LOW);
  digitalWrite(DISP1, LOW);
  delay(1000);

  //LOCK1
  digitalWrite(DISP0, HIGH);
  digitalWrite(DISP1, DATA & B10000000);
  delay(1000);
  digitalWrite(DISP0, LOW);
  digitalWrite(DISP1, LOW);
  delay(1000);

  //LOCK2
  digitalWrite(DISP0, HIGH);
  digitalWrite(DISP1, DATA & B01000000);
  delay(1000);
  digitalWrite(DISP0, LOW);
  digitalWrite(DISP1, LOW);
  delay(1000);

  //check every bit individually
  for (int i = 0, b = 1; i < 8; i++, b<<1)
  {
    digitalWrite(DISP0, HIGH);
    digitalWrite(DISP1, DATA & b);
    delay(50);
    digitalWrite(DISP0, LOW);
    delay(50);
  }
  
  digitalWrite(DISP1, LOW);

  //check all the bits
  delay(1000);
  digitalWrite(DISP0, HIGH);
  delay(500);
  digitalWrite(DISP1, DATA);
  delay(500);
  digitalWrite(DISP1, LOW);
  delay(500);
  digitalWrite(DISP0, LOW);
  delay(50);
  
  //dissable output
  digitalWrite(OE, HIGH);
  //digitalWrite(BS, HIGH);

  DATAD = 0xFF; // set digital pins 0-7 as outputs
}
*/



