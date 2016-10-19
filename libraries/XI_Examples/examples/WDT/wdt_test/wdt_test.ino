#include <LGTWDT.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);

  // delay to check wdt reset condition
  delay(1000);
  wdt_enable(WTO_128MS);
}

void loop() {
  // put your main code here, to run repeatedly:
  //wdt_reset();
  digitalWrite(0, HIGH);
  delay(1);
  digitalWrite(0, LOW);
  delay(1);
}
