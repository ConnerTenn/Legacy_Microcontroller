
#include <avr/io.h>
//#define DDRD _SFR_IO8(0x0A)
//#define PORTD _SFR_IO8(0x0B)

void Delay(long count)
{
	
	for (long i = 0; i < count; i++)
	{
		int a = 0;
	}
}

void SetPin(char pin, char value)
{
	unsigned char buf = PORTD;
	buf = buf & ~(1 << pin);
	buf = buf | (value << pin);
	PORTD = buf;
}

char ReadPin(char pin)
{
	return (PIND >> pin) & 0b1;
}

void main()
{
	//DDRA = 0xFF;
	//DDRB = 0xFF;
	//DDRC = 0xFF;
	DDRD = 0b11001111;
	//PORTD = 0b11111111;

	int i = 0;
	while (1)
	{
		SetPin(5, 1);
		Delay(100);
		SetPin(5, 0);
		SetPin(6, ReadPin(5));
		Delay(100);
	}
}
