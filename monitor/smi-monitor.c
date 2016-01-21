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
	int serialSwbWait=10;
	int serialSmiWait=40;
	int serialSwbCnt=0;
	int serialSmiCnt=0;
	int bytes_available=0;
	int fdSwb; /* File descriptor for the SWB-port */
	int fdSmi; /* File descriptor for the SMI-port */
	int x;
	int loop;
	int bytes=0;
////	int bytesSmi;
//	int bytesSwb=0;
	char c;
	int ch;
	char buffer[50];
	char bufferSwb[50];
	char bufferSmi[50];
	int bufferSwbCnt=0;
	int bufferSmiCnt=0;
   //char *bufptr;
    
   /* first parameter is serialSwb0Port*/
   if (argc > 1)
   {
   	serialSwbPort=argv[1];
   } else {
   	serialSwbPort=serialSwb0Port;
   }
   /* second parameter is serialSwbWait in ms*/
   if (argc > 2)
   {
   	serialSwbWait=atoi(argv[2]);
   } else {
   	serialSwbWait=5;
   }
   /* second parameter is serialSmi0Port*/
   if (argc > 3)
   {
   	serialSmiPort=argv[3];
   } else {
   	serialSmiPort=serialSmi0Port;
   }
   /* fourth parameter is serialSmiWait in ms*/
   if (argc > 4)
   {
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
  } 
  else
  {
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
	if (fdSmi == -1)
	{
	  /* Could not open the port. */
	  perror("Unable to open serial SMI-port");
	  return(-1);
	} 
	else
	{
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
serialSmiCnt=0;
serialSwbCnt=0;
for (loop=0; ; loop++)
    {
	
      	/* SWB-Bus */
	bytes_available=ioctl(fdSwb, FIONREAD, &ch);
	if (bytes_available>0)
	{
		if (serialSwbCnt==0)
		{
		 	bytes = read(fdSwb, &buffer, sizeof(buffer));
			bufferSwbCnt=bytes;
			memmove(bufferSwb, buffer, sizeof(buffer));
		}
		if ((serialSwbCnt>0) && (serialSwbCnt<serialSwbWait))
		{
		 	bytes = read(fdSwb, &buffer, sizeof(buffer));
			bufferSwbCnt+=bytes;
			memmove(bufferSwb+bufferSwbCnt, buffer, sizeof(buffer));
		}
	}
	if (serialSwbCnt>=serialSwbWait)
	{
		bytes_available=ioctl(fdSwb, FIONREAD, &ch);
		if (bytes_available>0)
		{
		 	bytes = read(fdSwb, &buffer, sizeof(buffer));
			bufferSwbCnt+=bytes;
			memmove(bufferSwb+bufferSwbCnt, buffer, sizeof(buffer));
		}
		if (bufferSwbCnt>0)
		{
			/* print message */
			printf("\n%4d Swb: ",loop);
			for (x = 0; x < (bufferSwbCnt) ; x++)
			{
				c = bufferSwb[x];
				printf("%02X ",c);
			}	
		}
		/* clear message */
		bufferSwbCnt=0;
		serialSwbCnt=0;
	}
	
    	/* SMI-Bus */
	bytes_available=0;
	bytes_available=ioctl(fdSmi, FIONREAD, &ch);
	if (bytes_available>0)
	{
		if (serialSmiCnt==0)
		{
		 	bytes = read(fdSmi, &buffer, sizeof(buffer));
			bufferSmiCnt=bytes;
			memmove(bufferSmi, buffer, sizeof(buffer));
		}
		if ((serialSmiCnt>0) && (serialSmiCnt<serialSmiWait))
		{
		 	bytes = read(fdSmi, &buffer, sizeof(buffer));
			bufferSmiCnt+=bytes;
			memmove(bufferSmi+bufferSmiCnt, buffer, sizeof(buffer));
		}
	}
	if (serialSmiCnt>=serialSmiWait)
	{
		bytes_available=ioctl(fdSmi, FIONREAD, &ch);
		if (bytes_available>0)
		{
		 	bytes = read(fdSmi, &buffer, sizeof(buffer));
			bufferSmiCnt+=bytes;
			memmove(bufferSmi+bufferSmiCnt, buffer, sizeof(buffer));
		}
		if (bufferSwbCnt>0)
		{
			/* print message */
			printf("\n%4d Smi: ",loop);
			for (x = 0; x < (bufferSmiCnt) ; x++)
			{
				c = bufferSmi[x];
				printf("%02X ",c);
			}
		}
		/* clear message */
		bufferSmiCnt=0;
		serialSmiCnt=0;
	}
	
	/* wait 100ms */
	usleep(100000);
	serialSwbCnt++;
	serialSmiCnt++;
	if (serialSwbCnt>serialSwbWait)
	{
		serialSwbCnt=0;
	}
	if (serialSmiCnt>serialSmiWait)
	{
		serialSmiCnt=0;
	}
	printf(".");


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

