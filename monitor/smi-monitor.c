#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting functions */
#include <sys/ioctl.h> /* ioctl() */

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	char * serialSwbPort="/dev/ttySWB1";
	char serialSwb0Port[]="/dev/ttySWB1";
	char * serialSmiPort="/dev/ttySMI0";
	char serialSmi0Port[]="/dev/ttySMI0";
	int serialSwbWait=2; //1;15ms
	int serialSmiWait=10; //5;49ms
	int serialSwbCount;
	int serialSmiCount;
	int actualSwbTimeout=0;
	int actualSmiTimeout=0;
	int IOReturn;

	int fdSwb; /* File descriptor for the SWB-port */
	int fdSmi; /* File descriptor for the SMI-port */
	int x;
	int loop;
//////	int bytes=0;
	int bytesSmi=0;
	int bytesSwb=0;
	char c;
	int serialBytes;
//	char buffer[50];
	char bufferSwb[50];
	char bufferSmi[50];
	int bufferSwbCount=0;
	int bufferSmiCount=0;
   //char *bufptr;

   /* first parameter is serialSwb0Port*/
   if (argc > 1) {
   	serialSwbPort=argv[1];
   } else {
   	serialSwbPort=serialSwb0Port;
   }
   /* second parameter is serialSwbWait in ms*/
   if (argc > 2) {
   	serialSwbWait=atoi(argv[2]);
   } else {
   	serialSwbWait=5;
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
   	serialSmiWait=40;
   }
   /*
	* 'open_port()' - Open serial port 1.
	* Returns the file descriptor on success or -1 on error.
	*/

  fdSwb = open(serialSwbPort, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (fdSwb == -1)
  {
  	/* Could not open the port. */
  	perror("Unable to open serial SWB-port");
  	return(-1);
  } else {
  	fcntl(fdSwb, F_SETFL, 0);
  }

struct termios options;

/* Get the current options for the SWB-port... */
tcgetattr(fdSwb, &options);

/* Set the baud rates to 19200... */
printf("SWB: 19.200 (%dms)\n",serialSwbWait);
cfsetispeed(&options, B19200);
cfsetospeed(&options, B19200);

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
tcsetattr(fdSwb, TCSANOW, &options);


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
serialSmiCount=0;
serialSwbCount=0;
loop=0;
for ( ; ; )
    {
	loop++;
	if (loop>=0x80000000) loop=0;

	/* SWB-Bus */
	IOReturn=ioctl(fdSwb, FIONREAD, &serialBytes);
	//if ((serialBytes+actualSwbTimeout)>0) printf("\nSWB loop: %d \tBytes: %d \tTimeout: %d \tCount: %d",loop/2,serialBytes,actualSwbTimeout,serialSwbCount);
//	printf(" \b");
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
			//		IOReturn=ioctl(fdSwb, FIONREAD, &serialBytes);
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
			printf("\n%10d SWB: ",loop);
			for (x = 0; x < (bufferSwbCount) ; x++)
			{
				c = bufferSwb[x];
				printf("%02X ",c);
			}
			printf("\033[m");
			bufferSwbCount=0;
		}
	}

    	/* SMI-Bus */
	IOReturn=ioctl(fdSmi, FIONREAD, &serialBytes);
	if ((serialBytes+actualSmiTimeout)>0) printf("\nSMI loop: %d \tBytes: %d \tTimeout: %d \tCount: %d",loop/2,serialBytes,actualSmiTimeout,serialSmiCount);
//	printf(" \b");
	if (IOReturn<0) {
		perror("ioctl(smi)");
		if (actualSmiTimeout>0) actualSmiTimeout--;
	}
	if (IOReturn==0) {
		if ((serialBytes==0)&&(actualSmiTimeout>0)) {
			actualSmiTimeout--;
		}
		if (serialBytes>0) {
			if ((actualSmiTimeout==0)&&(bufferSmiCount==0)) {
				/* start receiving and reset timeout */
				actualSmiTimeout=serialSmiWait;
			}
			if ((actualSmiTimeout>=0)&&(bufferSmiCount>=0)) {
				/* start receiving and reset timeout */
				actualSmiTimeout=serialSmiWait;
			}
			//		IOReturn=ioctl(fdSmi, FIONREAD, &serialBytes);
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
			printf("\n\033[1m%010d SMI: ",loop);
			for (x = 0; x < (bufferSmiCount) ; x++)
			{
				c = bufferSmi[x];
				printf("%02X ",c);
			}
			printf("\033[m");
			bufferSmiCount=0;
		}
	}
	
	/* SMI-Bus */
	//IOReturn=ioctl(fdSmi, FIONREAD, &ch);
	//IOReturn=-1;
//	if ((IOReturn>=0)&&(IOReturn<0))
//	{
//		printf("das sollte nie passieren SMI!!!\n");
//		bytesSmi = read(fdSmi, &buffer, sizeof(buffer));
//		if (bytesSmi>0)
//		{
//			bufferSmiCount+=bytesSmi;
//		}
//		memmove(bufferSmi+bufferSmiCount, buffer, sizeof(buffer));
//		if (bufferSmiCount>0)
//		{
//			printf("\033[1m\nSMI: ");
//			for (x = 0; x < (bytesSmi) ; x++)
//			{
//				c = bufferSmi[x];
//				printf("%02X ",c);
//			}
//			printf("\033[m");
//			bufferSmiCount=0;
//		}
//	}


	/* wait 0,5ms */
	usleep(500);
	serialSwbCount++;
	serialSmiCount++;
	if (serialSwbCount>serialSwbWait) {
		serialSwbCount=0;
	}
	if (serialSmiCount>serialSmiWait) {
		serialSmiCount=0;
	}
//	printf("2");

}

/* Close Ports */
   close(fdSwb);
   printf("<serial Swb port closed!\n");
   close(fdSmi);
   printf("<serial Swb port closed!\n");
   return (0);

}


int open_port(void)
{
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
