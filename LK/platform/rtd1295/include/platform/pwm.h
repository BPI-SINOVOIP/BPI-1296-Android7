#ifndef _RTK_PWM_H_
#define _RTK_PWM_H_

void rtd129x_pwm_init(void);
void rtd129x_pwm_show(void);
int pwm_enable(int pwm_number, unsigned int value);
int pwm_set_duty_rate(int pwm_number, unsigned int duty_rate);
int pwm_set_freq(int pwm_number, unsigned int freq);
int pwm_get_freq(int pwm_number, unsigned int* freq);

#endif // _RTK_PWM_H_
