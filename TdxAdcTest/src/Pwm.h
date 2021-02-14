/*
 * Pwm.h
 *
 *  Created on: 03-Aug-2020
 *      Author: jeet
 */

#ifndef PWM_H_
#define PWM_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

class Pwm {
public:
	Pwm();
	static int pwmWrite(char* path, int value);
	static int iniPwm(int ch);
	static int enablePwm(int ch, int on);
	static int setPwm(int ch, int dc, int pr);
	static int deinit(int ch);

};

#endif /* PWM_H_ */
