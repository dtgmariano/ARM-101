/*
 * timer.h
 *
 *  Created on: 4 de Mar de 2013
 *      Author: ufu
 */

#ifndef TIMER_H_
#define TIMER_H_

void timer0Match(int mr, unsigned long int freq, char mode);
void ISR_Timer0(void);

#endif /* TIMER_H_ */
