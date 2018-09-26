/*

*/

#ifndef _T3_PWM_H
#define _T3_PWM_H

#include <Arduino.h>

#define TC3_OC3AEN   1
#define TC3_C3AAC   1   // OC3A output to PD6
#define TC3_OC3BEN  1
#define TC3_OC3CEN  0
#define TC3_CS3   0x1   // SYSCLK
#define TC3_WGM3  0x8   // PFCPWM mode (TOP=ICR3)
#define TC3_COM3A   0x2   // disable comparator output
#define TC3_COM3B   0x2   // disable comparator output
#define TC3_COM3C   0x0
#define TC3_ICES3   0x0   // falling edge is used as trigger
#define TC3_ICNC3   0
#define TC3_OCR3A   100
#define TC3_OCR3B   50
#define TC3_ICR3  100


enum emCom3Nopwm
{
  E_COM3_NDIS = 0x0,  /**< disable OC */
  E_COM3_CTOG,    /**< toggle on compare match */
  E_COM3_CCLR,    /**< clear on comapre match */
  E_COM3_CSET     /**< set on compare match */
};


class T3_PWM 
{
  private:
    uint8_t t3_cs3=1;
    uint16_t t3_ocr_3a;
    uint16_t t3_ocr_3b;
    uint16_t t3_ocr_3c;
    uint16_t t3_icr3;
    uint32_t pwm_freq;
    float duty_a;
    float duty_b;
    float duty_c;

  public:
    T3_PWM();
    void init();
    void init(uint32_t freq);
    void changeFreq(uint32_t freq);
    void changeDuty(uint8_t pin, float duty);
    void stop();
    void start();

};

extern T3_PWM T3PWM;

#endif

