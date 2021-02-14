/*
 ============================================================================
 Name        : ads1115_example.c
 Author      : Giovanni Bauermeister
 Description : Read analog values from potentiometer using ADS1115 and prints to terminal
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "ads1115_rpi.h"
#include <time.h>

int poll(unsigned char dv, int channel) {

	float data[250];
   unsigned int config = 0;
   config = 	CONFIG_REG_OS_SINGLE	    |
				CONFIG_REG_PGA_4_096V 		|
				CONFIG_REG_MODE_SINGLE 		|
				CONFIG_REG_DR_32SPS		    |
				CONFIG_REG_CMODE_TRAD 		|
				CONFIG_REG_CPOL_ACTIV_LOW 	|
				CONFIG_REG_CLATCH_NONLATCH 	|
				CONFIG_REG_CQUE_NONE;

		clock_t clk = clock();

		printf("Polling Device:%d, Channel:%d -----------------------------------\n", dv, channel);

		if(openI2CBus("/dev/i2c-0") == -1)
		{
			return EXIT_FAILURE;
		}

		setI2CSlave(dv);

		readAdc(channel, config, 200, data, 1);


		for(int i=0; i<10; i++)
		{
			printf("Line:%d: ", (i));
			for(int j=0;j<20;j++){
				printf("%.3f | ",data[i*20 + j]);
			}
			printf("\n");
		}

		float mean = 0;

		for(int i=0; i<200 ; i++){
			mean += data[i];
		}
		mean /= 200;
		printf("Mean V:%.3f -----------------\n",mean);

		closeI2CBus();

		clock_t outclk = clock();

		printf("Time In%ld, Out%ld, Elapsed:%ld \n", clk, outclk, outclk-clk);

		return 0;
}


int pollall(unsigned char dv) {

	if(openI2CBus("/dev/i2c-0") == -1)
	{
		return EXIT_FAILURE;
	}

	setI2CSlave(dv);

	while(1)
	{
		printf("CH_0 = %.3f V | ", readVoltage(0));
		printf("CH_1 = %.3f V | ", readVoltage(1));
		printf("CH_2 = %.3f V | ", readVoltage(2));
		printf("CH_3 = %.3f V \n", readVoltage(3));
	}

	closeI2CBus();
}


int main(void) {
	//pollall(0x48);
	printf("Polling Unipolar  ---------\n");
	poll(0x48,0);
	//poll(0x48,1);
	//poll(0x48,2);
	//poll(0x48,3);
	sleep(1);
	printf("Polling Differential---------\n");
	poll(0x48,4);
	//poll(0x48,5);
/*
	poll(0x49,0);
	poll(0x49,1);
	poll(0x49,2);
	poll(0x49,3);
	poll(0x49,4);
	poll(0x49,5);
*/
	return EXIT_SUCCESS;
}
