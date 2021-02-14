/*
 * isl26104.c
 *
 *  Created on: 19-Aug-2020
 *      Author: 
 */


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/ioctl.h>
#include <sys/stat.h>
#include <linux/types.h>
#include <stdbool.h>
#include <linux/spi/spidev.h>



#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	if (errno != 0)
		perror(s);
	else
		printf("%s\n", s);

	abort();
}

static const char *device = "/dev/spidev1.1";
static uint32_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;
static int verbose;
static int transfer_size;
static int iterations;
static char *input_tx;

static void hex_dump(const void *src, size_t length, size_t line_size,
		     char *prefix)
{
	int i = 0;
	const unsigned char *address = src;
	const unsigned char *line = address;
	unsigned char c;

	printf("%s | ", prefix);
	while (length-- > 0) {
		printf("%02X ", *address++);
		if (!(++i % line_size) || (length == 0 && i % line_size)) {
			if (length == 0) {
				while (i++ % line_size)
					printf("__ ");
			}
			printf(" |");
			while (line < address) {
				c = *line++;
				printf("%c", (c < 32 || c > 126) ? '.' : c);
			}
			printf("|\n");
			if (length > 0)
				printf("%s | ", prefix);
		}
	}
}


static void transfer(int fd, uint8_t const *tx, uint8_t const *rx, size_t len)
{
	int ret;
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = len,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	//if (mode & SPI_TX_OCTAL)
	//	tr.tx_nbits = 8;
	//else
	if (mode & SPI_TX_QUAD)
		tr.tx_nbits = 4;
	else if (mode & SPI_TX_DUAL)
		tr.tx_nbits = 2;

	//if (mode & SPI_RX_OCTAL)
		//tr.rx_nbits = 8;
	//else
	if (mode & SPI_RX_QUAD)
		tr.rx_nbits = 4;
	else if (mode & SPI_RX_DUAL)
		tr.rx_nbits = 2;

	if (!(mode & SPI_LOOP)) {
		if (mode & (/*SPI_TX_OCTAL |*/ SPI_TX_QUAD | SPI_TX_DUAL))
			tr.rx_buf = 0;
		else if (mode & (/*SPI_RX_OCTAL |*/ SPI_RX_QUAD | SPI_RX_DUAL))
			tr.tx_buf = 0;
	}
	//JEET
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	if (ret < 1)
		pabort("can't send spi message");

	if (verbose)
		hex_dump(tx, len, 32, "TX");

	if (verbose)
		hex_dump(rx, len, 32, "RX");
}

static void print_usage(const char *prog)
{
	printf("Usage: %s [-DsbdlHOLC3vpNR24SI]\n", prog);
	puts("  -D --device   device to use (default /dev/spidev1.1)\n"
	     "  -s --speed    max speed (Hz)\n"
	     "  -d --delay    delay (usec)\n"
	     "  -b --bpw      bits per word\n"
	     "  -i --input    input data from a file (e.g. \"test.bin\")\n"
	     "  -o --output   output data to a file (e.g. \"results.bin\")\n"
	     "  -l --loop     loopback\n"
	     "  -H --cpha     clock phase\n"
	     "  -O --cpol     clock polarity\n"
	     "  -L --lsb      least significant bit first\n"
	     "  -C --cs-high  chip select active high\n"
	     "  -3 --3wire    SI/SO signals shared\n"
	     "  -v --verbose  Verbose (show tx buffer)\n"
	     "  -p            Send data (e.g. \"1234\\xde\\xad\")\n"
	     "  -N --no-cs    no chip select\n"
	     "  -R --ready    slave pulls low to pause\n"
	     "  -2 --dual     dual transfer\n"
	     "  -4 --quad     quad transfer\n"
	     "  -8 --octal    octal transfer\n"
	     "  -S --size     transfer size\n"
	     "  -I --iter     iterations\n");
	exit(1);
}

static void parse_opts(int argc, char *argv[])
{
	while (1) {
		static const struct option lopts[] = {
			{ "device",  1, 0, 'D' },
			{ "speed",   1, 0, 's' },
			{ "delay",   1, 0, 'd' },
			{ "bpw",     1, 0, 'b' },
			{ "input",   1, 0, 'i' },
			{ "output",  1, 0, 'o' },
			{ "loop",    0, 0, 'l' },
			{ "cpha",    0, 0, 'H' },
			{ "cpol",    0, 0, 'O' },
			{ "lsb",     0, 0, 'L' },
			{ "cs-high", 0, 0, 'C' },
			{ "3wire",   0, 0, '3' },
			{ "no-cs",   0, 0, 'N' },
			{ "ready",   0, 0, 'R' },
			{ "dual",    0, 0, '2' },
			{ "verbose", 0, 0, 'v' },
			{ "quad",    0, 0, '4' },
			{ "octal",   0, 0, '8' },
			{ "size",    1, 0, 'S' },
			{ "iter",    1, 0, 'I' },
			{ NULL, 0, 0, 0 },
		};
		int c;

		c = getopt_long(argc, argv, "D:s:d:b:i:o:lHOLC3NR248p:vS:I:",
				lopts, NULL);

		if (c == -1)
			break;

		switch (c) {
		case 'D':
			device = optarg;
			break;
		case 's':
			speed = atoi(optarg);
			break;
		case 'd':
			delay = atoi(optarg);
			break;
		case 'b':
			bits = atoi(optarg);
			break;
		case 'l':
			mode |= SPI_LOOP;
			break;
		case 'H':
			mode |= SPI_CPHA;
			break;
		case 'O':
			mode |= SPI_CPOL;
			break;
		case 'L':
			mode |= SPI_LSB_FIRST;
			break;
		case 'C':
			mode |= SPI_CS_HIGH;
			break;
		case '3':
			mode |= SPI_3WIRE;
			break;
		case 'N':
			mode |= SPI_NO_CS;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'R':
			mode |= SPI_READY;
			break;
		case 'p':
			input_tx = optarg;
			break;
		case '2':
			mode |= SPI_TX_DUAL;
			break;
		case '4':
			mode |= SPI_TX_QUAD;
			break;
		//case '8':
			//mode |= SPI_TX_OCTAL;
			//break;
		case 'S':
			transfer_size = atoi(optarg);
			break;
		case 'I':
			iterations = atoi(optarg);
			break;
		default:
			print_usage(argv[0]);
		}
	}
	if (mode & SPI_LOOP) {
		if (mode & SPI_TX_DUAL)
			mode |= SPI_RX_DUAL;
		if (mode & SPI_TX_QUAD)
			mode |= SPI_RX_QUAD;
		//if (mode & SPI_TX_OCTAL)
			//mode |= SPI_RX_OCTAL;
	}
}

//===================================
extern void GpioInit();
extern int setCS(bool on);
extern void GpioDeInit();
//===================================

#define islcmd_chipid 	0x00
#define islcmd_sdo 		0x02
#define islcmd_standby 	0x03
#define islcmd_owr		0x05
#define islcmd_imxsel 	0x07
#define islcmd_chptr 	0x08
#define islcmd_pgagain 	0x17
#define islcmd_ctrl 	0x04
#define islcmd_pgaoffsetH 0x3D
#define islcmd_pgaoffsetM 0x3E
#define islcmd_pgaoffsetL 0x3F
#define islcmd_pgaMon 	  0x3C
//
int isl_set(int fd, uint8_t cmd, uint8_t dt){
	uint8_t tx[4];
	uint8_t rx[4];
	tx[0] = cmd | 0x80; tx[1] = dt; tx[2] = 0xFF; tx[3] = 0xFF;
	setCS(false); //drive
	transfer(fd, tx, rx, 2);
	setCS(true); //drive
	return 0;
}

int isl_get(int fd, uint8_t cmd){
	uint8_t tx[4];
	uint8_t rx[4];
	tx[0] = cmd; tx[1] = 0xFF; tx[2] = 0xFF; tx[3] = 0xFF;
	setCS(false); //drive
	transfer(fd, tx, rx, 2);
	setCS(true); //drive
	int i = (int)rx[1];
	return i;
}

int isl_get_adc(int fd)
{
	uint8_t tx[4];
	uint8_t rx[4];
	tx[0] = 0xFF; tx[1] = 0xFF; tx[2] = 0xFF; tx[3] = 0xFF;
	transfer(fd, tx, rx, 3);
	int i = 0;

	i = (((int)rx[0]) << 16) + (((int)rx[1]) << 8) + ((int)rx[2]);
	if(rx[0] > 127) i |= 0xFF000000;
	return i;
}


void islTestFun(int fd)
{
	GpioInit();

	printf("Reading ID...........................\n");

	for(int i=0; i<3; i++)
	{
		int id = isl_get(fd, islcmd_chipid);
		printf("Chipid: %d \n", id);
		usleep(10000);
	}

	printf("Setting Input Ch ptr ......\n");
	isl_set(fd, islcmd_chptr, 0x05); //temp
	//isl_set(fd, islcmd_chptr, 0x03); //ch4
	usleep(10000);

	printf("Setting Mux to Temperature Channel......\n");
	isl_set(fd, islcmd_imxsel, 0x05); //temp
	//printf("Setting Mux to Channel4......\n");
	//isl_set(fd, islcmd_imxsel, 0x03); //Channel1

	usleep(10000);
	printf("Read Mux setting ......\n");
	int ms = isl_get(fd, islcmd_imxsel);
	printf("MuxSetting: %d \n", ms);
	usleep(10000);

	printf("Setting PGA Gain ......\n");
	isl_set(fd, islcmd_pgagain, 0x00); // pga gain 1
	usleep(10000);
	printf("Reading PGA Gain ......\n");
	int pa = isl_get(fd, islcmd_pgagain);
	printf("PGA Gain: %d \n", pa);
	usleep(10000);

	printf("Setting op word rate ......\n");
	isl_set(fd, islcmd_owr, 0x06); // 160ps
	usleep(10000);
	printf("Setting op word rate ......\n");
	int owr = isl_get(fd, islcmd_owr);
	printf("OWR: %d \n", owr);
	usleep(10000);

	for(int j = 0; j<20; j++)
	{
		printf("Single Con Start ......\n");
		isl_set(fd, islcmd_ctrl, 0x01);
		usleep(9900);

		/*
		for(int k = 0; k<10; k++)
		{
			printf("Check if Conversion Finished ? \n");
			if(isl_get(fd, islcmd_ctrl) == 0x00){
				printf("Conversion Finished \n");
				break;
			}
			usleep(10000);
		}
		*/

		printf("Read ADC ......\n");
		int adc = isl_get_adc(fd);
		printf("ADC Value: %d \n", adc);
		double d = 0.00019669532 *  ((double)adc);
		printf("MilliVolts:%1.3lf\n",d);
		double tmp = (d - 102.2) / 0.379;
		printf("Temp:%1.2lf\n",tmp);
		usleep(100);
	}

	GpioDeInit();
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int fd;

	parse_opts(argc, argv);

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE32, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: 0x%x\n", mode);
	printf("bits per word: %u\n", bits);
	printf("max speed: %u Hz (%u kHz)\n", speed, speed/1000);

	islTestFun(fd);

	close(fd);
}
