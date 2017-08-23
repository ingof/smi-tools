#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting functions */

#include <sys/ioctl.h>		/* ioctl() */
#include <linux/serial.h>	/* custom divisor */

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	char * serialPort="/dev/ttyUSB0";
	char serialPort2[]="/dev/ttyUSB0";
	int serialSpeed=2400;
	int serialWait=3;

	int actualHexTimeout;

	int IOReturn;


	int fd; /* File descriptor for the port */

	int serialBytes;
	int x;
	int loop;
	int loop2;
	int c;

	int bytesHex=0;
	int bufferHexCount=0;
	unsigned char bufferHex[50];
	unsigned char tmpBuffer[40];

   //char *bufptr;

   /* first parameter is serialSpeed*/
   if (argc > 1)
   {
   	serialSpeed=atoi(argv[1]);
   } else {
   	/* default speed for serial port */
   	serialSpeed=2400;
   }
   /* second parameter is serialPort*/
   if (argc > 2)
   {
   	serialPort=argv[2];
   } else {
   	serialPort=serialPort2;
   }
   /* third parameter is serialWait in ms*/
   if (argc > 3)
   {
   	serialWait=atoi(argv[3]);
   } else {
   	serialWait=40;
   }
	/*
	* 'open_port()' - Open serial port 1.
	* Returns the file descriptor on success or -1 on error.
	*/

  fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (fd == -1)
  {
  	/* Could not open the port. */
  	perror("Unable to open serial-port");
  	return(-1);
  }
  else
  {
  	fcntl(fd, F_SETFL, 0);
  }

struct termios options;

/* Get the current options for the port... */
tcgetattr(fd, &options);

if (serialSpeed == 2400)
{
	/* Set the baud rates to 2400... */
	printf("2.400\n");
	cfsetispeed(&options, B2400);
	cfsetospeed(&options, B2400);
}
if (serialSpeed == 9600)
{
	/* Set the baud rates to 9600... */
	printf("9.600\n");
	cfsetispeed(&options, B9600);
	cfsetospeed(&options, B9600);
}
if (serialSpeed == 19200)
{
	/* Set the baud rates to 19200... */
	printf("19.200\n");
	cfsetispeed(&options, B19200);
	cfsetospeed(&options, B19200);
}

/* Enable the receiver and set local mode... */
options.c_cflag |= (CLOCAL | CREAD);

/* Setting Character Size */
options.c_cflag &= ~CSIZE; /* Mask the character size bits */
options.c_cflag |= CS8;    /* Select 8 data bits */


/* Setting 8N1 */
options.c_cflag &= ~PARENB; 	/* deactivate Parity */
options.c_cflag &= ~CSTOPB;		/* two stop bits */
options.c_cflag &= ~CSIZE;		/* mask for data bits */
options.c_cflag |= CS8;    		/* Select 8 data bits */
options.c_iflag &= ~IXON;		/* deactivate XON */
options.c_iflag &= ~IXOFF;		/* deactivate XOFF */
options.c_iflag &= ~IGNCR;		/* do NOT ignore CR */
options.c_iflag &= ~ICRNL;		/* do NOT replace CR with NL */
options.c_iflag &= ~INLCR;		/* do NOT replace NL with CL */
options.c_iflag |= IGNBRK;		/* ignore break condition (SWB) */

/* choosing RAW-Input */
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

/* Set the new options for the port... */
tcsetattr(fd, TCSANOW, &options);

/* endless-loop */
for (loop=0; ; loop+=(serialWait*2)) {
	if (loop>=0x80000000) {
		loop=0;
	}
	// bytes = read(fd, &buffer, sizeof(buffer));
	IOReturn=ioctl(fd, FIONREAD, &serialBytes);
	if (IOReturn<0) {
		perror("ioctl(hex)");
		if (actualHexTimeout>0) actualHexTimeout--;
	}
	if (IOReturn==0) {
		/* no data -< */
		if ((serialBytes==0)&&(actualHexTimeout>0)) {
			actualHexTimeout--;
		}
		/* copy received data to buffer */
		if (serialBytes>0) {
			if ((actualHexTimeout==0)&&(bufferHexCount==0)) {
				/* start receiving and reset timeout */
				actualHexTimeout=serialWait;
			}
			if ((actualHexTimeout>=0)&&(bufferHexCount>=0)) {
				/* start receiving and reset timeout */
				actualHexTimeout=serialWait;
			}
			/* create temporary buffer for received Bytes */
			// int tmpBuffer[serialBytes];
			bytesHex = read(fd, &tmpBuffer, sizeof(tmpBuffer));
			if (bytesHex<0) {
				perror("read(Hex)");
			}
			if (bytesHex<=0) {
				actualHexTimeout--;
			}
			if (bytesHex>0) {
				// strncpy(bufferSmi+bufferSmiCount, tmpBuffer, bytesSmi);
				for (loop2=0;loop2<bytesHex;loop2++) {
					bufferHex[bufferHexCount+loop2]=tmpBuffer[loop2];
				}
				bufferHexCount+=bytesHex;
			}
		}
		/* stop receiving and print message */
		if ((actualHexTimeout==0)&&(bufferHexCount>0)) {
			printf("\n\033[1m%6d.%03d Hex:\033[0m ",loop/2000,(loop/2)%1000);
			for (x = 0; x < (bufferHexCount-1) ; x++)
			{
				c = bufferHex[x];
				printf("%02X ",c);
			}

			bufferHexCount=0;
			fflush(stdout); // Will now print everything in the stdout buffer
		}
	}
	/* wait (SerialWait)ms" */
	usleep(500);
	// usleep(serialWait*1000);
	// bytes = read(fd, &buffer, sizeof(buffer));
	// if (bytes == -1) {
	// 	perror ("read error:");
	// } else {
	// 	for (x = 0; x < bytes ; x++)
	// 	{
	// 		c = (buffer[x] & 0xff);
	// 		printf("%02X ",c);
	// 	}
	// printf("\n");
	// }

}

/* Close Port */
   close(fd);
   printf("<serial port closed!\n");
   return (0);

}


int open_port(void)
{
	int fd; /* File descriptor for the port */
	fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		/* Could not open the port. */
		perror("Unable to open /dev/ttyUSB0 - ");

	}
	else
	{
		fcntl(fd, F_SETFL, FNDELAY);
	}
	return (fd);
}

// int setNonblocking(int fd)
// {
//     int flags;
//
//     /* If they have O_NONBLOCK, use the Posix way to do it */
// #if defined(O_NONBLOCK)
//     /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
//     if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
//         flags = 0;
//     return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
// #else
//     /* Otherwise, use the old way of doing it */
//     flags = 1;
//     return ioctl(fd, FIOBIO, &flags);
// #endif
// }
