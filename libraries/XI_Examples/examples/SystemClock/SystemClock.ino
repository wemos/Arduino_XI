//============================================
//sysClock demo
//
//============================================

uint16_t value;

void setup() {

	Serial.begin(19200);
  
}

void loop() {

	sysClock(EXT_CLOCK); // switch to external crystal
	Serial.println("Switch to external crystal clock.");
	Serial.print("PMCR=0x");
	Serial.println(PMCR,HEX);
	Serial.print("CLKPR=0x");
	Serial.println(CLKPR,HEX);
	Serial.println();
	delay(2000);

	sysClock(IN_CLOCK); // switch to inside RC CLock
	Serial.println("Switch to inside RC cLock.");
	Serial.print("PMCR=0x");
	Serial.println(PMCR,HEX);
	Serial.print("CLKPR=0x");
	Serial.println(CLKPR,HEX);
	Serial.println();
	delay(2000);
}

