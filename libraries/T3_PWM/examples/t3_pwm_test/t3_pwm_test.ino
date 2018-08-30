#include <T3_PWM.h>

uint32_t freq;
float duty;

void setup() {

  Serial.begin(115200);

  T3PWM.init();
  T3PWM.changeDuty(2, 50);
  T3PWM.changeDuty(6,0);
}

void loop() {
  // put your main code here, to run repeatedly:

  while (Serial.available() > 0)
  {

    freq = (uint32_t)Serial.parseInt();
    duty = Serial.parseFloat();

    Serial.print("freq = ");
    Serial.println(freq, DEC);

    Serial.print("Duty = ");
    Serial.print(duty, DEC);
    Serial.println("%");


    T3PWM.changeFreq(freq);
    T3PWM.changeDuty(2, duty);
  }

}
