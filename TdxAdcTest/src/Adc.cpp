/*
 * Adc.cpp
 *
 *  Created on: 03-Aug-2020
 *
 */

#include "Adc.h"

Adc::Adc() {
	// TODO Auto-generated constructor stub
}


int Adc::adcRead(int ch, float &value)
{
		#define VALUE_MAX 50
	    char path[VALUE_MAX];
	    char value_str[3];
	    int fd;

	    if(ch >= 4 ) return -2;
	    if(ch >1 ) ch += 6;

	    snprintf(path, VALUE_MAX, "/sys/bus/iio/devices/iio\:device0/in_voltage%d_raw", ch);

	    fd = open(path, O_RDONLY);
	    if (-1 == fd) {
	        fprintf(stderr, "Failed to open adc %d for reading!\n",ch);
	        return(-1);
	    }

	    if (-1 == read(fd, value_str, 5)) {
	        fprintf(stderr, "Failed to read value!\n");
	        return(-1);
	    }

	    close(fd);
	    int v = atoi(value_str);
	    value = (3.3 * float(v))/ 4096;
	    return 0;
}

