/*
 * Pwm.cpp
 *
 *  Created on: 03-Aug-2020
 *      Author: jeet
 */

#include "Pwm.h"

Pwm::Pwm() {
	// TODO Auto-generated constructor stub
}

int Pwm::pwmWrite(char* path, int value)
{
	int fd;
	int r;
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open %s PWM Channel for writing!\n", path);
		return(-1);
	}

	char val[10];
	snprintf(val,10,"%d",value);

	r = write (fd, val, strlen(val));
	if(-1 == r) {
		fprintf(stderr, "Failed to write on %s\n", path);
		return(-1);
	}

	close(fd);
	return(0);
}

int Pwm::iniPwm(int ch)
{
	char path[50];
	snprintf(path, 50, "/sys/class/pwm/pwmchip%d/export",(ch));
	pwmWrite(path,0);
	return 0;
}
int Pwm::deinit(int ch)
{
	char path[50];
	snprintf(path, 50, "/sys/class/pwm/pwmchip%d/unexport",(ch));
	pwmWrite(path,0);
	return 0;
}

int Pwm::enablePwm(int ch, int on)
{
	char path[60];
    snprintf(path, 60, "/sys/class/pwm/pwmchip%d/pwm0/enable",(ch));
	if(on)pwmWrite(path,1);
	else pwmWrite(path,0);
	return 0;
}

int Pwm::setPwm(int ch, int dc, int pr)
{
    char path[60];
    snprintf(path, 60, "/sys/class/pwm/pwmchip%d/pwm0/period",(ch));
    pwmWrite(path,pr);
    snprintf(path, 60, "/sys/class/pwm/pwmchip%d/pwm0/duty_cycle",(ch));
    pwmWrite(path,dc);
    return(0);
}
