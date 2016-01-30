#include <stdio.h>			/* Standard input/output definitions */
#include <string.h>			/* String function definitions */
#include <unistd.h>			/* UNIX standard function definitions */
#include <fcntl.h>			/* File control definitions */
#include <errno.h>			/* Error number definitions */
#include <termios.h>		/* POSIX terminal control definitions */
#include <stdlib.h>			/* converting functions */
#include <sys/ioctl.h>		/* ioctl() */
#include "smi-monitor.h"	/* own funcions */

#include <sys/types.h>
#include <sys/stat.h>
// #include <sys/ioctl.h>
#include <sys/time.h>//#include <serial.h>
#include <linux/serial.h> /* custom divisor */
// typedef unsigned char uint8_t;
// typedef unsigned int  uint16_t;
//
// void addSwbCrc(char *buffer, int size);

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	char * serialSwbPort="/dev/ttySWB0";
	char serialSwb0Port[]="/dev/ttySWB0";
	char * serialSmiPort="/dev/ttySMI0";
	char serialSmi0Port[]="/dev/ttySMI0";
	int serialSwbWait=3; //1,5ms; 15ms
	int serialSmiWait=10; //5ms ; 40ms
	int actualSwbTimeout=0;
	int actualSmiTimeout=0;
	int IOReturn;
	int serialSwb0Speed=1200;
	int fdSwb; /* File descriptor for the SWB-port */
	int fdSmi; /* File descriptor for the SMI-port */
	int serialBytes;
	int x;
	int loop;
	char c;

	int bytesSwb=0;
	char bufferSwb[50];
	int bufferSwbCount=0;
	int bytesSmi=0;
	char bufferSmi[50];
	int bufferSmiCount=0;

	// temporary test use:
	char tmp2Buf[50];
	int tmp2Ret;
	int tmp2Siz;
	int tmp2Err;
	// char tmp3Buf[50];


	/* first parameter is serialSwb0Port*/
	if (argc > 1) {
		serialSwb0Speed=atoi(argv[1]);
	} else {
		serialSwb0Speed=1200;
	}
	// /* first parameter is serialSwb0Port*/
	// if (argc > 1) {
	// 	serialSwbPort=argv[1];
	// } else {
		serialSwbPort=serialSwb0Port;
	// }
	/* second parameter is serialSwbWait in ms*/
	if (argc > 2) {
		serialSwbWait=atoi(argv[2]);
	} else {
		serialSwbWait=3;
	}
	/* second parameter is serialSmi0Port*/
	if (argc > 3) {
		serialSmiPort=argv[3];
	} else {
		serialSmiPort=serialSmi0Port;
	}
	/* fourth parameter is serialSmiWait in ms*/
	if (argc > 4) {
		serialSmiWait=atoi(argv[4]);
		} else {
		serialSmiWait=10;
	}
	/*
	* 'open_port()' - Open serial port 1.
	* Returns the file descriptor on success or -1 on error.
	*/

	fdSwb = open(serialSwbPort, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (fdSwb == -1) {
		/* Could not open the port. */
		perror("Unable to open serial SWB-port");
		return(-1);
	} else {
		fcntl(fdSwb, F_SETFL, 0);
	}

	struct termios options;

	struct serial_struct ser;

	/* Get the current options for the SWB-port... */
	if (tcgetattr(fdSwb, &options)<0) perror("tcGetattr");

	cfsetispeed(&options, B38400);
	cfsetospeed(&options, B38400);
	// cfsetispeed(&options, B19200);
	// cfsetospeed(&options, B19200);

	/* Set the baud rates to 25000... */
	if (ioctl(fdSwb, TIOCGSERIAL, &ser)<0) perror("tioGserial");
	// ser.flags=(ser.flags&(~ASYNC_SPD_MASK));
	ser.flags |= ASYNC_SPD_CUST;
	/* divisor for 25000 kBit/s (alias 38400) */
	ser.custom_divisor=serialSwb0Speed;
	if (ioctl(fdSwb, TIOCSSERIAL, &ser)<0) perror("tioSserial");

	printf("SWB: %d (%dms, div:%d)\n",(30000000/serialSwb0Speed),serialSwbWait,serialSwb0Speed);

	/* Enable the receiver and set local mode... */
	options.c_cflag |= (CLOCAL | CREAD);

	/* Setting Character Size */
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */

	/* Setting 8N1 */
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/* choosing RAW-Input */
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* Set the new options for the port... */
	if (tcsetattr(fdSwb, TCSANOW, &options)<0) perror("tcsetattr");


	fdSmi = open(serialSmiPort, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if (fdSmi == -1) {
		/* Could not open the port. */
		perror("Unable to open serial SMI-port");
		return(-1);
	} else {
		fcntl(fdSmi, F_SETFL, 0);
	}


	/* Get the current options for the SMI-port... */
	tcgetattr(fdSmi, &options);

	/* Set the baud rates to 2400... */
	printf("SMI:  2.400 (%dms)\n",serialSmiWait);
	cfsetispeed(&options, B2400);
	cfsetospeed(&options, B2400);

	/* Enable the receiver and set local mode... */
	options.c_cflag |= (CLOCAL | CREAD);

	/* Setting Character Size */
	options.c_cflag &= ~CSIZE; /* Mask the character size bits */
	options.c_cflag |= CS8;    /* Select 8 data bits */

	/* Setting 8N1 */
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;

	/* choosing RAW-Input */
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	/* Set the new options for the port... */
	tcsetattr(fdSmi, TCSANOW, &options);


	/* endless-loop */
	for (loop=0; ;loop++) {
		if (loop>=0x80000000) loop=0;

		// tmp3Buf[0]=0xf0;
		// tmp3Buf[1]=0xaa;
		// tmp3Buf[2]=0xaa;
		// tmp3Buf[3]=0xaa;
		// tmp3Buf[4]=0xaa;
		// tmp3Buf[5]=0xaa;
		// tmp3Buf[6]=0xaa;
		//
		// write(fdSwb,&tmp3Buf,7);



		/* SWB-Bus */
		IOReturn=ioctl(fdSwb, FIONREAD, &serialBytes);
		// IOReturn=0;
		if (IOReturn<0) {
			perror("ioctl(swb)");
			if (actualSwbTimeout>0) actualSwbTimeout--;
		}
		if (IOReturn==0) {
			if ((serialBytes==0)&&(actualSwbTimeout>0)) {
				actualSwbTimeout--;
			}
			if (serialBytes>0) {
				if ((actualSwbTimeout==0)&&(bufferSwbCount==0)) {
					/* start receiving and reset timeout */
					actualSwbTimeout=serialSwbWait;
				}
				if ((actualSwbTimeout>=0)&&(bufferSwbCount>=0)) {
					/* start receiving and reset timeout */
					actualSwbTimeout=serialSwbWait;
				}
				/* create temporary buffer for received Bytes */
				int tmpBuffer[serialBytes];
				bytesSwb = read(fdSwb, &tmpBuffer, sizeof(tmpBuffer));
				if (bytesSwb<0) {
					perror("read(Swb)");
				}
				if (bytesSwb<=0) {
					actualSwbTimeout--;
				}
				if (bytesSwb>0) {
					memmove(bufferSwb+bufferSwbCount, tmpBuffer, bytesSwb);
					bufferSwbCount+=bytesSwb;
				}
			}
			/* stop receiving and print message */
			if ((actualSwbTimeout==0)&&(bufferSwbCount>0)) {
				printf("\n%6d.%03d SWB: ",loop/2000,(loop/2)%1000);
				for (x = 0; x < (bufferSwbCount-2) ; x++)
				{
					c = bufferSwb[x];
					printf("%02X ",c);
				}
				memmove(tmp2Buf,bufferSwb,bufferSwbCount-2);
				tmp2Siz=bufferSwbCount;
				switch (checkSwbCrc(bufferSwb,bufferSwbCount)) {
					case -2:
						/* crc 2 not ok -> yellow */
						printf("\033[1m");
						tmp2Ret=-2;
						break;
					case -1:
						/* crc 1 not ok -> red */
						printf("\033[31m");
						tmp2Ret=-1;
						break;
					default:
						/* crc is ok -> green */
						printf("\033[32m");
						tmp2Ret=0;
						break;
				}
				c = bufferSwb[bufferSwbCount-2];
				printf("%02X ",c);
				c = bufferSwb[bufferSwbCount-1];
				printf("%02X ",c);

				printf("\033[m");
				bufferSwbCount=0;

				// if (tmp2Ret==0) {
				// 	// createSwbAck(tmp2Buf,tmp2Siz);
				// 	// write(fdSwb,&tmp2Buf,tmp2Siz);
				// 	// write(fdSwb,&tmp2Buf,7);
				// } else {
				// 	// tmp2Err++;
				// 	// if (tmp2Err<=10) {
				// 	// } else {
				// 	// 	printf("\007\033[m\033[41m\033[1m   !  S T O P  !\033[40m\033[m");
				// 	// 	tmp2Err=0;
				// 	// 	// createSwbAck(tmp2Buf,tmp2Siz);
				// 	// 	// write(fdSwb,&tmp2Buf,tmp2Siz);
				// 	// 	//write(fdSwb,&tmp2Buf,7);
				// 	// }
				// }
				printf("\033[31m   *  N A C K  *\033[m");
				if (ioctl(fdSwb, TIOCGSERIAL, &ser)<0) perror("tioGserial");
				// ser.flags=(ser.flags&(~ASYNC_SPD_MASK));
				ser.flags |= ASYNC_SPD_CUST;
				/* divisor for 25000 kBit/s (alias 38400) */
				ser.custom_divisor=serialSwb0Speed++;
				if (ioctl(fdSwb, TIOCSSERIAL, &ser)<0) perror("tioSserial");
				printf("  *%d*  ",serialSwb0Speed);

				fflush(stdout); // Will now print everything in the stdout buffer
			}
		}

		/* SMI-Bus */
		IOReturn=ioctl(fdSmi, FIONREAD, &serialBytes);
		// IOReturn=0;
		if (IOReturn<0) {
			perror("ioctl(smi)");
			if (actualSmiTimeout>0) actualSmiTimeout--;
		}
		if (IOReturn==0) {
			/* no data -< */
			if ((serialBytes==0)&&(actualSmiTimeout>0)) {
				actualSmiTimeout--;
			}
			/* copy received data to buffer */
			if (serialBytes>0) {
				if ((actualSmiTimeout==0)&&(bufferSmiCount==0)) {
					/* start receiving and reset timeout */
					actualSmiTimeout=serialSmiWait;
				}
				if ((actualSmiTimeout>=0)&&(bufferSmiCount>=0)) {
					/* start receiving and reset timeout */
					actualSmiTimeout=serialSmiWait;
				}
				/* create temporary buffer for received Bytes */
				int tmpBuffer[serialBytes];
				bytesSmi = read(fdSmi, &tmpBuffer, sizeof(tmpBuffer));
				if (bytesSmi<0) {
					perror("read(Smi)");
				}
				if (bytesSmi<=0) {
					actualSmiTimeout--;
				}
				if (bytesSmi>0) {
					memmove(bufferSmi+bufferSmiCount, tmpBuffer, bytesSmi);
					bufferSmiCount+=bytesSmi;
				}
			}
			/* stop receiving and print message */
			if ((actualSmiTimeout==0)&&(bufferSmiCount>0)) {
				printf("\n\033[1m%6d.%03d SMI: ",loop/2000,(loop/2)%1000);
				for (x = 0; x < (bufferSmiCount-1) ; x++)
				{
					c = bufferSmi[x];
					printf("%02X ",c);
				}
				// c = bufferSmi[bufferSmiCount];
				switch (checkSmiCrc(bufferSmi,bufferSmiCount)) {
					case -2:
						/* frame without CRC -> yellow */
						printf("\033[1m");
						break;
					case -1:
						/* crc not ok -> red */
						printf("\033[31m");
						break;
					default:
						/* crc is ok -> green */
						printf("\033[32m");
						break;
				}
				c = bufferSmi[bufferSmiCount-1];
				printf("%02X \033[m",c);
				bufferSmiCount=0;
				fflush(stdout); // Will now print everything in the stdout buffer
			}
		}

		/* wait 0,5ms */
		usleep(500);

	}

	/* Close Ports */
	close(fdSwb);
	printf("<serial Swb port closed!\n");
	close(fdSmi);
	printf("<serial Swb port closed!\n");
	return (0);

}


uint16_t  createSwbCrc(char *buffer, int size)
{
	uint16_t crc = 0xffff;    // preset CRC
	uint16_t CRC = 0x8408;    // for reverse calculation of CRC-16-CCITT
	int i,j;

	for (i=0; i < size-2; i++){
		crc = crc ^ buffer[i];
		for (j=0; j<8; j++){
			if((crc & 0x01) == 0){
				crc = crc >> 1;
			} else {
				crc = crc >> 1;
				crc = crc ^ CRC;
			}
		}
	}
 return ~crc;
}


void printBuffer(char *buffer, int size) {
	printf("  (%d",size);
	int x;
	char c2;
	for (x = 0; x < (size) ; x++)
	{
		c2 = buffer[x];
		printf("%02X.",c2);
	}
	printf(")");
	fflush(stdout); // Will now print everything in the stdout buffer
}

/* create response of received message */
void createSwbAck(char *buffer, int size) {
	// fix for short messages:
	if (size==6) size++;
	// clear switch-byte
	buffer[size-3]=0;
	// clear old crc
	buffer[size-2]=0;
	buffer[size-1]=0;
	// display old message
	int crc=createSwbCrc(buffer, size);
	buffer[size-2]=(uint8_t) crc;
	buffer[size-1]=(uint8_t) (crc>>8);
	// display generated acknolegde
	printBuffer(buffer, size);
}

/* add the crc to an existing message */
void addSwbCrc(char *buffer, int size) {
	buffer[size]=0;
	buffer[size+1]=0;
	// printBuffer(buffer, size);
	int crc=createSwbCrc(buffer, size);
	buffer[size]=(uint8_t) crc;
	buffer[size+1]=(uint8_t) (crc>>8);
	// printBuffer(buffer, size+2);
}

/* check SwitchBus crc-16 */
int  checkSwbCrc(char *buffer, int size) {
	int crc=createSwbCrc(buffer, size);
	if (buffer[size-2]!=(uint8_t) crc) {
		printf("(-1:%02x%02x) ",  (uint8_t) crc, (uint8_t) (crc>>8));
		return -1;
	}
	if (buffer[size-1]!=(uint8_t) (crc>>8)) {
		printf("(-2:%02x%02x) ",  (uint8_t) crc, (uint8_t) (crc>>8));
		return -2;
	}
	printf("( 0:%02x%02x) ",  (uint8_t) crc, (uint8_t) (crc>>8));
	return 0;
}


int checkSmiCrc(char *buffer, int size) {
	if (size<=2) {
		return -2;
	}
	/* create checksum */
	char tmpChkSum=0;
	int i;
	for (i = 0; i < size-1; i++) {
		tmpChkSum+=buffer[i];
	}
	tmpChkSum=(~tmpChkSum)+1;
	if (buffer[size-1]!=tmpChkSum) {
		return -1;
	}
	return 0;
}

int open_port(void) {
	int fd; /* File descriptor for the port */
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1) {
		/* Could not open the port. */
		perror("Unable to open /dev/ttyUSB0 - ");
	} else {
		fcntl(fd, F_SETFL, FNDELAY);
	}
	return (fd);
}
