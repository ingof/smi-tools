#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting functions */

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	char * serialSwbPort="/dev/ttySWB0";
	char serialSwb0Port[]="/dev/ttySWB0";
	char * serialSmiPort="/dev/ttySMI0";
	char serialSmi0Port[]="/dev/ttySMI0";
	int serialSwbWait=5;
	int serialSmiWait=40;
	int serialSwbCount;
	int serialSmiCount;
	
	int fdSwb; /* File descriptor for the SWB-port */
	int fdSmi; /* File descriptor for the SMI-port */
	int x;
	int loop;
	int bytes;
	char c;
	char bufferSwb[20];
	char bufferSmi[20];
	int bufferSwbCnt;
	int bufferSmiCnt;
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

  fdSwb = open(serialSwbPort, O_RDWR | O_NOCTTY | O_NDELAY);
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
printf("SWB: 19.200\n");
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


	fdSmi = open(serialSmiPort, O_RDWR | O_NOCTTY | O_NDELAY);
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
printf("SMI: 2.400\n");
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
for (loop=0; ; loop++)
    {
    	/* SWB-Bus */
	if (serialSwbCount==0)
	{
		bytes = read(fdSwb, &bufferSwb, sizeof(bufferSwb));
	    	serialSwbCount=bytes;
		if (bytes == -1)
		{
			perror ("read error swb:");
		}
	}
	
	if (serialSwbCount>=serialSwbWait)
	{
		printf("%4d SWB: ", loop);
		for (x = 0; x < (bytes) ; x++)
		{
			c = bufferSwb[x];
			if (bufferSwb[x]==0xf0) {
				printf("\n          ");
			}
			printf("%02X ",c);
		}
		bytes = read(fdSwb, &bufferSwb, sizeof(bufferSwb));
		if (bytes == -1)
		{
			perror ("read error swb:");
		}
		else
		{
			for (x = 0; x < (bytes) ; x++)
			{
				c = bufferSwb[x];
				if (bufferSwb[x]==0xf0) {
					printf("\n          ");
				}
				printf("%02X ",c);
			}
			printf("\n");
			serialSwbCount=-1;
		}
	}
	
    	/* SMI-Bus */
	if (serialSmiCount==0)
	{
		bytes = read(fdSwb, &bufferSmi, sizeof(bufferSmi));
	    	serialSmiCount=bytes;
		if (bytes == -1)
		{
			perror ("read error swb:");
		}
		else
		{
			loop++;
		}
	}
	
	if (serialSmiCount>=serialSmiWait)
	{
		printf("%4d SWB: ", loop);
		for (x = 0; x < (bytes) ; x++)
		{
			c = bufferSmi[x];
			if (bufferSmi[x]==0xf0) {
				printf("\n          ");
			}
			printf("%02X ",c);
		}
		bytes = read(fdSmi, &bufferSmi, sizeof(bufferSmi));
		if (bytes == -1)
		{
			perror ("read error smi:");
		}
		else
		{
			for (x = 0; x < (bytes) ; x++)
			{
				c = bufferSmi[x];
				if (bufferSmi[x]==0xf0) {
					printf("\n          ");
				}
				printf("%02X ",c);
			}
			printf("\n");
			serialSmiCount=-1;
		}
	}
	/* wait 1ms */
	usleep(1000);
	serialSwbCount++;
	serialSmiCount++;

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
