/*

*/

#include "T1_PWM.h"

T1_PWM::T1_PWM()
{
}

void T1_PWM::init()
{
  init(1000);
}

void T1_PWM::init(uint32_t freq)
{

  pwm_freq = freq;
  // u8reg is used for storing value of SREG
  u8 u8reg;

  /** 1. Stop timer */
  TCCR1B = 0x0;

  /** 2. Force compare match: OC = 1*/
  // The setup of the OC1x should be performed before setting
  // the Data Direction Register for the port pin to output
  // set OC1x before DDR_OC1x is set
  TCCR1A = (E_COM1_CSET << COM1A0) | (E_COM1_CSET << COM1B0);
  // force compare match
  TCCR1C = 0xc0;

  /** 3. Set PIN OC3x's direction */
  // #if (TC3_OC3AEN == 1)
  // #if (TC3_C3AAC == 1)
  //   u8reg = PMX1 | 0x4;
  //   PMX0 = 0x80;
  //   PMX1 = u8reg;
  // #else
  //   DDRF |= (1 << PF1);
  // #endif
  // #endif

  // #if (TC3_OC3BEN == 1) || (TC3_OC3CEN == 1)
  //   DDRF |= (TC3_OC3BEN << PF2) | (TC3_OC3CEN << PF3);
  // #endif

  /** 4. Disalble Global Interrupt */
  u8reg = SREG;
  cli();

  /** 5. Initiate TCNT3 */
  // TCNT3H = (TC3_TCNT3 >> 8) & 0xff;
  // TCNT3L = TC3_TCNT3 & 0xff;

  /** 6. Initiate (OCR1A & OCR1B)output compare register */
  // OCR3AH = (TC3_OCR3A >> 8) & 0xff;
  // OCR3AL = TC3_OCR3A & 0xff;
  // OCR3BH = (TC3_OCR3B >> 8) & 0xff;
  // OCR3BL = TC3_OCR3B & 0xff;

  // ICR3H = (TC3_ICR3 >> 8) & 0xff;
  // ICR3L = TC3_ICR3 & 0xff;

  changeFreq(freq);

  /** 7. Restore SREG  */
  SREG = u8reg;

  /** 9. Initiate TCCR3A and TCCR3B */
  TCCR1A = (0 << COM1A0) | (0 << COM1B0) | ((TC1_WGM1 & 0x3) << WGM10);
}

void T1_PWM::changeFreq(uint32_t freq)
{
  if (freq > 80000)
    pwm_freq = 80000;
  else
    pwm_freq = freq;

  if (pwm_freq == 0) //stop pwm
  {
    t1_cs1 = 0;
    t1_icr1 = 0;
  }
  else if (pwm_freq == 1) //256 div
  {
    t1_cs1 = 4;
    t1_icr1 = 16000000 / 2 / 256 / pwm_freq;
  }

  else if (pwm_freq < 100) //64 div
  {
    t1_cs1 = 3;
    t1_icr1 = 16000000 / 2 / 64 / pwm_freq;
  }

  else if (pwm_freq < 1000) //8 div
  {
    t1_cs1 = 2;
    t1_icr1 = 16000000 / 2 / 8 / pwm_freq;
  }

  else //1 div
  {
    t1_cs1 = 1;
    t1_icr1 = 16000000 / 2 / pwm_freq;
  }

  TCCR1B = t1_cs1 | (((TC1_WGM1 & 0xc) >> 2) << WGM12) | (TC1_ICNC1 << ICNC1) | (TC1_ICES1 << ICES1);

  ICR1H = (t1_icr1 >> 8) & 0xff;
  ICR1L = t1_icr1 & 0xff;

  t1_ocr_1a = t1_icr1 * duty_a / 100;
  OCR1AH = (t1_ocr_1a >> 8) & 0xff;
  OCR1AL = t1_ocr_1a & 0xff;

  t1_ocr_1b = t1_icr1 * duty_b / 100;
  OCR1BH = (t1_ocr_1b >> 8) & 0xff;
  OCR1BL = t1_ocr_1b & 0xff;
}

void T1_PWM::changeDuty(uint8_t pin, float duty)
{
  float _duty;
  u8 u8reg;

  if (duty > 100)
  {
    _duty = 100;
  }
  else
  {
    _duty = duty;
  }

  switch (pin)
  {

#if defined(__LGT8F_SSOP20__)

  case 22: //PB6,OC1A

    duty_a = _duty;

    
    u8reg = PMX0;
    DDRF |= (1 << PF5);
    u8reg |= (1 << C1AF5);
    PMX0 = 0x80;
    PMX0 = u8reg;

    t1_ocr_1a = t1_icr1 * duty_a / 100;
    OCR1AH = (t1_ocr_1a >> 8) & 0xff;
    OCR1AL = t1_ocr_1a & 0xff;

    TCCR1A |= (2 << COM1A0);

    break;

  case 9: //PB1,OC1A

    duty_a = _duty;

    DDRB |= (1 << 1); //PB1

    t1_ocr_1a = t1_icr1 * duty_a / 100;
    OCR1AH = (t1_ocr_1a >> 8) & 0xff;
    OCR1AL = t1_ocr_1a & 0xff;

    TCCR1A |= (2 << COM1A0);

    break;

  case 11: //PB3,OC1B

    duty_b = _duty;

    DDRB |= (1 << 2); //PB2

    t1_ocr_1b = t1_icr1 * duty_b / 100;
    OCR1BH = (t1_ocr_1b >> 8) & 0xff;
    OCR1BL = t1_ocr_1b & 0xff;

    TCCR1A |= (2 << COM1B0);

    break;

  case 3: //PD3,OC1B

    duty_b = _duty;

    u8reg = PMX0;
    DDRF |= (1 << PF4);
    u8reg |= (1 << C1BF4);
    PMX0 = 0x80;
    PMX0 = u8reg;

    t1_ocr_1b = t1_icr1 * duty_b / 100;
    OCR1BH = (t1_ocr_1b >> 8) & 0xff;
    OCR1BL = t1_ocr_1b & 0xff;

    TCCR1A |= (2 << COM1B0);

    break;

#endif
  }
}

T1_PWM T1PWM;
