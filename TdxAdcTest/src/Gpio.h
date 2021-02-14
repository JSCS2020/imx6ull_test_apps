/*
 * Gpio.h
 *
 *  Created on: 02-Aug-2020
 *      Author: jeet
 */

#ifndef GPIO_H_
#define GPIO_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

//#define GPIO_LOG

class Gpio {
public:
	Gpio();
	static int GPIOUnexport(int pin);
	static int GPIOExport(int pin);
	static int GPIODirection(int pin, int dir);
	static int GPIORead(int pin);
	static int GPIOWrite(int pin, bool value);
};

#endif /* GPIO_H_ */

