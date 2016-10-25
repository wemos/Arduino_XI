#include <avr/io.h>
#include <stdio.h>
#include <stdarg.h>

#include "Arduino.h"


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

		CLKPR = 0x80;
		CLKPR = 0x00;
	}
	else
	{
		//switch to inside RC CLock
	}
}
