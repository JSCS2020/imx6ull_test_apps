/*
 *
 *  Created on: 02-Aug-2020
 *      Author: jeet
 */

#ifndef IOS_H_
#define IOS_H_

#include "Gpio.h"
#include "Adc.h"
#include "Pwm.h"

/*
*/

#define Di1 24  // pin 8  - sod-135 - GPIO1.24 - gpio24
#define Di2 122 // pin 9  - sod-98  - GPIO4.26 - gpio122
#define Di3 110 // pin 10 - sod-133 - GPIO4.14 - gpio110
#define Di4 118 // pin 11 - sod-103 - GPIO4.22 - gpio118
#define Di5 117 // pin 12 - sod-101 - GPIO4.21 - gpio117
#define Di6 120 // pin 13 - sod-97  - GPIO4.24 - gpio120
#define Di7 123	// pin 14 - sod-85  - GPIO4.27 - gpio123
#define Di8 119 // pin 15 - sod-79  - GPIO4.23 - gpio119

// pin 16 - RELK1 - sod-45 -  GPIO5.1 - gpio129
#define DO_RELK2 32 // pin 17 - RELK2 - sod-55 (default can) - GPIO2.0 - gpio32
// pin 18 - ITMR - sod-63 (default can)  - GPIO2.1 - gpio33

// pin 20-21,22,23 spi,mosi,miso,clk,cs
// --UART1A--------------
#define DO_SIMCD1 88 //pin 25 - sod-37 - RI  - gpio88
// pin 26,27 tx,rx
#define DO_RELK1 13 // pin 28 - sod-31 - DCD - gpio13
#define DO_ITMR 87  // pin 29 - sod-29 - DSR - gpio87
// pin 30,31 RTS,CTS
#define DO_SIMCD2 12 // pin 32 - sod-23 - DTR - gpio12

// pin PWM a,b,c,d 41,42,43,44
// pin 47,48,49,50 ADC in 0,1,2,3
// pin 1. +5V
// GND,7,19,24,33,40,45

class Ios {
public:
	Ios();
	void init();
	void deinit();

	bool isDi1High();
	bool isDi2High();
	bool isDi3High();
	bool isDi4High();
	bool isDi5High();
	bool isDi6High();
	bool isDi7High();
	bool isDi8High();

	float  adc1();
	float  adc2();
	float  adc3();
	float  adc4();
	float  adc5();
	float  adc6();
	float  adc7();
	float  adc8();

	void setRELK2(bool on);
	void setRELK1(bool on);
	void setITMR(bool on);
	void setSimCD1(bool on);
	void setSimCD2(bool on);

	bool setPwm1(int);
	bool setPwm2(int);
	bool setPwm3(int);
	bool setPwm4(int);
};

#endif /* IOS_H_ */
