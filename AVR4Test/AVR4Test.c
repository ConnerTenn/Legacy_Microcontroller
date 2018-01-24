
#include <avr/io.h>
//#define DDRD _SFR_IO8(0x0A)
//#define PORTD _SFR_IO8(0x0B)


void main()
{
	//DDRA = 0xFF;
	//DDRB = 0xFF;
	//DDRC = 0xFF;
	DDRD = 0xFF;

	int i = 0;
	while (1)
	{
		if (i >= 10000)
		{
			i = 0;
			//PORTA = ~PORTA;
			//PORTB = ~PORTB;
			//PORTC = ~PORTC;
			PORTD = ~PORTD;
		}
		i++;
	}
}
