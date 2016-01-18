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
	int serialWait=40;
	
	int fdSwb; /* File descriptor for the SWB-port */
	int fdSmi; /* File descriptor for the SMI-port */
	int x;
	int loop;
	int bytes;
	char c;
	char buffer[20];
   //char *bufptr;
    
   /* first parameter is serialSwb0Port*/
   if (argc > 1)
   {
   	serialSwbPort=argv[1];
   } else {
   	serialSwbPort=serialSwb0Port;
   }
   /* second parameter is serialSmi0Port*/
   if (argc > 2)
   {
   	serialSmiPort=argv[2];
   } else {
   	serialSmiPort=serialSmi0Port;
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
for (loop=0; ; loop++)
    {
    	/* SWB-Bus */
    	bytes = read(fdSwb, &buffer, sizeof(buffer));
    	printf("%4d SWB: ", loop);
	if (bytes == -1)
	{
		perror ("read error swb:");
	}
	else
	{
		for (x = 0; x < bytes ; x++)
		{
			c = buffer[x];
			printf("%02X ",c);
		}
	}
	/* wait 5ms */
//	usleep(serialWait*1000);
	usleep(5*1000);
	bytes = read(fdSwb, &buffer, sizeof(buffer));
	if (bytes == -1)
	{
		perror ("read error swb:");
	}
	else
	{
		for (x = 0; x < bytes ; x++)
		{
			c = buffer[x];
			printf("%02X ",c);
		}
	printf("\n");
	}
	
	loop++
	
    	/* SMI-Bus */
    	bytes = read(fdSmi, &buffer, sizeof(buffer));
    	printf("%4d SMI: ", loop);
	if (bytes == -1)
	{
		perror ("read error smi :");
	}
	else
	{
		for (x = 0; x < bytes ; x++)
		{
			c = buffer[x];
			printf("%02X ",c);
		}
	}
	/* wait 40ms */
	usleep(serialWait*1000);
	bytes = read(fdSmi, &buffer, sizeof(buffer));
	if (bytes == -1)
	{
		perror ("read error smi:");
	}
	else
	{
		for (x = 0; x < bytes ; x++)
		{
			c = buffer[x];
			printf("%02X ",c);
		}
	printf("\n");
	}
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
