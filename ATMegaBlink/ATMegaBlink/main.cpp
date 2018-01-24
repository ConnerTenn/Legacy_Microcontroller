/*
 * ATMegaBlink.cpp
 *
 * Created: 01-Apr-2017 1:19:01 AM
 * Author : Conner
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	DDRD = 0x00;
	int i = 0;
    while (true) 
    {
		i++;
		if (i > 10000)
		{
			PORTD = PORTD ? 0x00 : 0xFF;
		}
    }
}

