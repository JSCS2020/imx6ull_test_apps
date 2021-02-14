/*
 * Adc.h
 *
 *  Created on: 03-Aug-2020
 *      Author: jeet
 */

#ifndef ADC_H_
#define ADC_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Adc {
public:
	Adc();
	static int adcRead(int ch, float &value);
};

#endif /* ADC_H_ */
