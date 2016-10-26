#include <avr/io.h>
#include <stdio.h>
#include <stdarg.h>

#include "Arduino.h"

/*
clk: 
	1 - switch to external crystal
	0 - switch to inside RC CLock
*/
void sysClock(uint8_t clk)
{
	volatile unsigned char btmp = 0;
	
	if(clk)
	{		
		// switch to external crystal
		btmp = PMCR | 0x04;
		PMCR = 0x80;
		PMCR = btmp;

		for(btmp=0xf; btmp > 0; --btmp);
		btmp = PMCR | 0x20;
		PMCR = 0x80;
		PMCR = btmp;

		// 1 system clock
		CLKPR = 0x80;
		CLKPR = 0x00;
	}
	else
	{
		// 1/2 system clock
		CLKPR = 0x80;
		CLKPR = 0x01;

		//switch to inside RC CLock
		btmp = PMCR & 0x1f;   //0001 1111
		PMCR = 0x80;
		PMCR = btmp;

		// CLose the external crystal clock
		btmp = PMCR & 0x7B;   //0111 1011
		PMCR = 0x80;
		PMCR = btmp;
	}
}
