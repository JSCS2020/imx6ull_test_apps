//============================================================================
// Name        : TdxAdcTest.cpp
// Author      : -NA-
// Version     :
// Copyright   : CopyRight
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <errno.h>
#include <getopt.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
using namespace std;
#include "Adc.h"
#include "Ios.h"

int channel = 0;
int samples = 100;
int delay = 100;
int verbose = 0;
int toggle = 0;

static void print_usage(const char *prog)
{
	printf("Usage: %s [-csdvt]\n", prog);
	puts("  -c --channel   channel no (default 1)\n"
	     "  -s --samples   samples \n"
	     "  -d --interval  Interval(usec) between two samples \n"
		 "  -t --iotoggle  Toggle io during sampling \n"
	     "  -v --verbose  Verbose (show tx buffer)\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "channel",  1, 0, 'c' },
			{ "samples",  1, 0, 's' },
			{ "interval", 1, 0, 'd' },
			{ "verbose",  0, 0, 'v' },
			{ "iotoggle", 0, 0, 't' },
			{ NULL, 0, 0, 0 },
		};

		int c;

		c = getopt_long(argc, argv, "c:s:d:vt", lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'c':
			channel = atoi(optarg);
			break;
		case 's':
			samples = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'v':
			verbose = 1;
			break;
		case 't':
			toggle = 1;
			break;

		default:
			print_usage(argv[0]);
		}
	}
}

double getms()
{
	struct timespec tp;
	clock_gettime (CLOCK_MONOTONIC, &tp);
	double ms  = (((double)tp.tv_nsec) / 1000000) + (tp.tv_sec * 1000);
	return ms;
}

double getms_from(double msf)
{
	struct timespec tp;
	clock_gettime (CLOCK_MONOTONIC, &tp);
	double ms  = (((double)tp.tv_nsec) / 1000000) + (tp.tv_sec * 1000);
	return ms - msf;
}

int main(int argc, char *argv[])
{
	parse_opts(argc, argv);
	cout << "Toradex Imx6ull Internal ADC Test Application !!!" << endl; // prints !!!Hello World!!!

	Ios ios;

	float v1[100],v2[100],v3[100],v4[100];

	double begin = getms();

	for(int i = 0; i<samples; i++)
	{
		double start;
		if(verbose) start = getms_from(begin);

		if(toggle)ios.setITMR(true);
		if(channel < 4){
			Adc::adcRead(channel, v1[i]);
			if(toggle)ios.setITMR(false);
			if(verbose)printf("Ch%d:%0.3f V | ",channel,v1[i]);
		}
		else{
			Adc::adcRead(0, v1[i]);
			Adc::adcRead(1, v2[i]);
			Adc::adcRead(2, v3[i]);
			Adc::adcRead(3, v4[i]);
			if(toggle)ios.setITMR(false);
			if(verbose) printf("Ch1:%0.3f V | Ch2:%0.3f V | Ch3:%0.3f V | Ch4:%0.3f V | ",v1[i],v2[i],v3[i],v4[i]);
		}

		if(verbose){
			double etime = getms_from(begin);
			printf("Sample %03d, Start@: %3.3lf ms, Time Elapsed %3.3lf ms\n", i+1, start, etime-start);
		}

		if(delay > 0)usleep(delay);
	}

	printf("Total Time Elapsed %02.3lf ms\n", getms_from(begin));;

	if(verbose == 0){
		for(int i=0; i<samples; i++ ){
			if(channel >=4 ) printf("Ch1:%0.3f V | Ch2:%0.3f V | Ch3:%0.3f V | Ch4:%0.3f V \n",v1[i],v2[i],v3[i],v4[i]);
			else printf("Ch%d:%0.3f V \n",channel,v1[i]);
		}
	}

	ios.deinit();
	return 0;
}
