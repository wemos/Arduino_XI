/*

*/

#ifndef _T1_PWM_H
#define _T1_PWM_H

#include <Arduino.h>

#define TC1_OC1BEN 	0
#define TC1_C1BIO 	0  	// OC1B output to PB3
#define TC1_OC1AEN 	1
#define TC1_C1AIO 	1  	// OC1A output to PB6
#define TC1_CSX 	0x1  	// using T1CLK (= SYSCLK)
#define TC1_ICP1EN 	0
#define TC1_CS1 	0x1  	// T1CLK
#define TC1_WGM1 	0x8  	// PFCPWM mode (TOP=ICR1)
#define TC1_COM1A 	0x2  	// clear on compare match
#define TC1_COM1B 	0x0  	// disable comparator output
#define TC1_ICES1 	0x0  	// falling edge is used as trigger
#define TC1_ICNC1 	0
#define TC1_OCR1A 	0x0000
#define TC1_OCR1B 	0x0000
#define TC1_ICR1 	0x0000
#define TC1_TCNT1 	0x0000
#define TC1_TOV1EN 	0
#define TC1_OCF1AEN 	0
#define TC1_OCF1BEN 	0


enum emCom1Nopwm
{
	E_COM1_NDIS = 0x0,	/**< disable OC */
	E_COM1_CTOG,		/**< toggle on compare match */
	E_COM1_CCLR,		/**< clear on comapre match */
	E_COM1_CSET			/**< set on compare match */
};


class T1_PWM 
{
  private:
    uint8_t t1_cs1=1;
    uint16_t t1_ocr_1a;
    uint16_t t1_ocr_1b;
    uint16_t t1_icr1;
    uint32_t pwm_freq;
    float duty_a;
    float duty_b;
    float duty_c;

  public:
    T1_PWM();
    void init();
    void init(uint32_t freq);
    void changeFreq(uint32_t freq);
    void changeDuty(uint8_t pin, float duty);
    void stop();
    void start();

};

extern T1_PWM T1PWM;

#endif

