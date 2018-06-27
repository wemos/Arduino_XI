//============================================
// DAC0 output demo 
//============================================

unsigned char value = 0;

void setup() {
  // put your setup code here, to run once:
  analogReference(INTERNAL2V56);
  pinMode(DAC0, ANALOG);

  analogWrite(DAC0, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(DAC0, value);
  delay(10);
  value += 10;
}

