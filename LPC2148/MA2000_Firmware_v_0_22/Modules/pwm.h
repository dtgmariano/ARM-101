/*
 * pwm.h
 *
 *  Created on: 04/12/2012
 *      Author: Daniel
 */

#ifndef PWM_H_
#define PWM_H_

void PWMInit();
void setDutyCycle(unsigned int channel, unsigned int dutyccycle);
void setPWM_DOF1(unsigned int frequency);
void setDutyCycle_DOF1(unsigned int freq, unsigned int dutycycle);

#endif /* PWM_H_ */
