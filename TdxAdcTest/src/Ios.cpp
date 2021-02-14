/*
 * Ios.cpp
 *
 *  Created on: 02-Aug-2020
 *      Author: jeet
 */

#include "Ios.h"

Ios::Ios()
{
	init();
	//Pwm::iniPwm(0); reserve
	Pwm::iniPwm(1);
	Pwm::iniPwm(2);
	Pwm::iniPwm(3);
}

void Ios::init()
{
	Gpio::GPIOUnexport(Di1); Gpio::GPIOExport(Di1); Gpio::GPIODirection(Di1, IN);
	Gpio::GPIOUnexport(Di2); Gpio::GPIOExport(Di2); Gpio::GPIODirection(Di2, IN);
	Gpio::GPIOUnexport(Di3); Gpio::GPIOExport(Di3); Gpio::GPIODirection(Di3, IN);
	Gpio::GPIOUnexport(Di4); Gpio::GPIOExport(Di4); Gpio::GPIODirection(Di4, IN);
	Gpio::GPIOUnexport(Di5); Gpio::GPIOExport(Di5); Gpio::GPIODirection(Di5, IN);
	Gpio::GPIOUnexport(Di6); Gpio::GPIOExport(Di6); Gpio::GPIODirection(Di6, IN);
	Gpio::GPIOUnexport(Di7); Gpio::GPIOExport(Di7); Gpio::GPIODirection(Di7, IN);
	Gpio::GPIOUnexport(Di8); Gpio::GPIOExport(Di8); Gpio::GPIODirection(Di8, IN);

	Gpio::GPIOUnexport(DO_RELK1); Gpio::GPIOExport(DO_RELK1); Gpio::GPIODirection(DO_RELK1, OUT);
	Gpio::GPIOUnexport(DO_RELK2); Gpio::GPIOExport(DO_RELK2); Gpio::GPIODirection(DO_RELK2, OUT);
	Gpio::GPIOUnexport(DO_ITMR);  Gpio::GPIOExport(DO_ITMR);  Gpio::GPIODirection(DO_ITMR, OUT);

	Gpio::GPIOUnexport(DO_SIMCD1); Gpio::GPIOExport(DO_SIMCD1); Gpio::GPIODirection(DO_SIMCD1, OUT);
	Gpio::GPIOUnexport(DO_SIMCD2); Gpio::GPIOExport(DO_SIMCD2); Gpio::GPIODirection(DO_SIMCD2, OUT);
}

void Ios::deinit()
{

			Pwm::deinit(1);
			Pwm::deinit(2);
			Pwm::deinit(3);

			Gpio::GPIOUnexport(Di1);
			Gpio::GPIOUnexport(Di2);
			Gpio::GPIOUnexport(Di3);
			Gpio::GPIOUnexport(Di4);
			Gpio::GPIOUnexport(Di5);
			Gpio::GPIOUnexport(Di6);
			Gpio::GPIOUnexport(Di7);
			Gpio::GPIOUnexport(Di8);

			Gpio::GPIOUnexport(DO_RELK1);
			Gpio::GPIOUnexport(DO_RELK2);
			Gpio::GPIOUnexport(DO_ITMR);

			Gpio::GPIOUnexport(DO_SIMCD1);
			Gpio::GPIOUnexport(DO_SIMCD2);

}


void Ios::setRELK1(bool on){
	Gpio::GPIOWrite(DO_RELK1,on);
}

void Ios::setRELK2(bool on){
	Gpio::GPIOWrite(DO_RELK2,on);
}
void Ios::setITMR(bool on){
	Gpio::GPIOWrite(DO_ITMR,on);
}
void Ios::setSimCD1(bool on){
	Gpio::GPIOWrite(DO_SIMCD1,on);
}

void Ios::setSimCD2(bool on){
	Gpio::GPIOWrite(DO_SIMCD2,on);
}

bool Ios::setPwm1(int dc){
	return true;
}

bool Ios::setPwm2(int dc){
	if(dc > 100) dc = 100;
	if(dc < 0) dc  = 0;
	Pwm::setPwm(1,(10000*dc),1000000);
	Pwm::enablePwm(1,1);
	return true;
}

bool Ios::setPwm3(int dc){
	if(dc > 100) dc = 100;
	if(dc < 0) dc  = 0;
	Pwm::setPwm(2,(10000*dc),1000000);
	Pwm::enablePwm(2,1);
	return true;
}

bool Ios::setPwm4(int dc){
	if(dc > 100) dc = 100;
	if(dc < 0) dc  = 0;
	Pwm::setPwm(3,(10000*dc),1000000);
	Pwm::enablePwm(3,1);
	return true;
}

float Ios::adc1(){
	float value=0;
	Adc::adcRead(0,value);
	return value;
}

float Ios::adc2(){
	float value=0;
	Adc::adcRead(1,value);
	return value;
}

float Ios::adc3(){
	float value=0;
	Adc::adcRead(2,value);
	return value;
}

float Ios::adc4(){
	float value=0;
	Adc::adcRead(3,value);
	return value;
}


bool Ios::isDi1High(){
	return (Gpio::GPIORead(Di1) == 1);
}
bool Ios::isDi2High(){
	return (Gpio::GPIORead(Di2) == 1);
}
bool Ios::isDi3High(){
	return (Gpio::GPIORead(Di3) == 1);
}
bool Ios::isDi4High(){
	return (Gpio::GPIORead(Di4) == 1);
}
bool Ios::isDi5High(){
	return (Gpio::GPIORead(Di5) == 1);
}
bool Ios::isDi6High(){
	return (Gpio::GPIORead(Di6) == 1);
}
bool Ios::isDi7High(){
	return (Gpio::GPIORead(Di7) == 1);
}
bool Ios::isDi8High(){
	return (Gpio::GPIORead(Di8) == 1);
}
