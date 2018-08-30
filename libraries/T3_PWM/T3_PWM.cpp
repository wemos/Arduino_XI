/*

*/

#include "T3_PWM.h"

T3_PWM::T3_PWM()
{
}

void T3_PWM::init()
{
  init(1000);
}

void T3_PWM::init(uint32_t freq)
{

  pwm_freq = freq;
  // u8reg is used for storing value of SREG
  u8 u8reg;

  /** 1. Stop timer */
  TCCR3B = 0x0;

  /** 2. Force compare match: OC = 1*/
  // The setup of the OC1x should be performed before setting
  // the Data Direction Register for the port pin to output
  // set OC1x before DDR_OC1x is set
  TCCR3A = (E_COM3_CSET << COM3A0) | (E_COM3_CSET << COM3B0);
  // force compare match
  TCCR3C = 0xc0;

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
  TCCR3A = (0 << COM3A0) | (0 << COM3B0) | (0 << COM3C0) |
           ((TC3_WGM3 & 0x3) << WGM30);

  //TCCR3B = t3_cs3 | (((TC3_WGM3 & 0xc) >> 2) << WGM32) | (TC3_ICNC3 << ICNC3) | (TC3_ICES3 << ICES3);
}

void T3_PWM::changeFreq(uint32_t freq)
{
  if (freq > 80000)
    pwm_freq = 80000;
  else
    pwm_freq = freq;

  if (pwm_freq == 0) //stop pwm
  {
    t3_cs3 = 0;
    t3_icr3 = 0;
  }
  else if (pwm_freq == 1) //256 div
  {
    t3_cs3 = 4;
    t3_icr3 = 16000000 / 2 / 256 / pwm_freq;
  }

  else if (pwm_freq < 100) //64 div
  {
    t3_cs3 = 3;
    t3_icr3 = 16000000 / 2 / 64 / pwm_freq;
  }

  else if (pwm_freq < 1000) //8 div
  {
    t3_cs3 = 2;
    t3_icr3 = 16000000 / 2 / 8 / pwm_freq;
  }

  else //1 div
  {
    t3_cs3 = 1;
    t3_icr3 = 16000000 / 2 / pwm_freq;
  }

  // t3_icr3 = 16000000 / 2 / (8^(t3_cs3-1)) / pwm_freq;
  TCCR3B = t3_cs3 | (((TC3_WGM3 & 0xc) >> 2) << WGM32) | (TC3_ICNC3 << ICNC3) | (TC3_ICES3 << ICES3);

  ICR3H = (t3_icr3 >> 8) & 0xff;
  ICR3L = t3_icr3 & 0xff;

  t3_ocr_3a = t3_icr3 * duty_a / 100;
  OCR3AH = (t3_ocr_3a >> 8) & 0xff;
  OCR3AL = t3_ocr_3a & 0xff;

  t3_ocr_3b = t3_icr3 * duty_b / 100;
  OCR3BH = (t3_ocr_3b >> 8) & 0xff;
  OCR3BL = t3_ocr_3b & 0xff;
}

void T3_PWM::changeDuty(uint8_t pin, float duty)
{
  float _duty;
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
  case 2://PD2,OC3B

    duty_b = _duty;

    DDRF |= (TC3_OC3BEN << PF2);

    t3_ocr_3b = t3_icr3 * duty_b / 100;
    OCR3BH = (t3_ocr_3b >> 8) & 0xff;
    OCR3BL = t3_ocr_3b & 0xff;

    TCCR3A |= (2 << COM3B0);
    break;

  case 6://PD6, OC3A

    duty_a = _duty;

    u8 u8reg;
    u8reg = PMX1 | 0x4;
    PMX0 = 0x80;
    PMX1 = u8reg;

    t3_ocr_3a = t3_icr3 * duty_a / 100;
    OCR3AH = (t3_ocr_3a >> 8) & 0xff;
    OCR3AL = t3_ocr_3a & 0xff;

    TCCR3A |= (2 << COM3A0);
    break;
#endif
  }
}

T3_PWM T3PWM;
