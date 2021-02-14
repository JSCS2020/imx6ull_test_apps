/*
 * Gpio.cpp
 *
 *  Created on: 02-Aug-2020
 *      Author: jeet
 */

#include "Gpio.h"

Gpio::Gpio(){
	// TODO Auto-generated constructor stub
}

int Gpio::GPIOExport(int pin)
{
#define BUFFER_MAX 5
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open %d export for writing!\n",pin);
        return(-1);
    }
    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

/*
    Un-Export gpio
*/

int Gpio::GPIOUnexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open %d unexport for writing!\n",pin);
        return(-1);
    }
    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    return(0);
}

/*
    Set the direction of gpio
*/
int Gpio::GPIODirection(int pin, int dir)
{

#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;

    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open %d gpio direction for writing!\n", pin);
        return(-1);
    }

    int r;
    if(dir == 0) r = write(fd, "in", 2);
    else r = write(fd, "low", 3);
    if(-1 == r) {
        fprintf(stderr, "Failed to set direction!\n");
        return(-1);
    }
    close(fd);
    return(0);
}



int Gpio::GPIORead(int pin)
{
#define VALUE_MAX 30
    char path[VALUE_MAX];
    char value_str[3];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);

#ifdef GPIO_LOG
    printf("\n%s\n",path);
#endif

    fd = open(path, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open %s gpio value for reading!\n",pin);
        return(-1);
    }

    if (-1 == read(fd, value_str, 3)) {
        fprintf(stderr, "Failed to read value!\n");
        return(-1);
    }

    close(fd);
    return(atoi(value_str));
}


int Gpio::GPIOWrite(int pin, bool value)
{
    static const char s_values_str[] = "01";

    #define VALUE_MAX 30

    char path[VALUE_MAX];
    int fd;

    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);

#ifdef GPIO_LOG
    printf("\n%s\n",path);
#endif

    fd = open(path, O_WRONLY);
    if (-1 == fd) {
        fprintf(stderr, "Failed to open %d gpio value for writing!\n",pin);
        return(-1);
    }

    if (1 != write(fd, &s_values_str[LOW == value ? 0 : 1], 1)) {
        fprintf(stderr, "Failed to write value!\n");
        return(-1);
    }

    close(fd);
    return(0);
}
