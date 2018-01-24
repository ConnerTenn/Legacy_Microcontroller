/*
 * First Test.cpp
 *
 * Created: 01-Apr-2017 12:24:33 AM
 * Author : Conner
 */ 

#include <avr/io.h>


int main(void)
{
    /* Replace with your application code */
	int i = 1;
    while (true) 
    {
		i++;
		if (i >= 100000)
		{
			i = 0;
			PORTA = !PORTA;
		}
    }
}

