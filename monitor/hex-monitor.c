#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting functions */

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	char * serialPort="/dev/ttyUSB0";
	char serialPort2[]="/dev/ttyUSB0";
	int serialSpeed=2400;
	int serialWait=40;
	
	int fd; /* File descriptor for the port */
	int x;
	int loop;
	int bytes;
	int c;
	char buffer[20];
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

  fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
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
options.c_cflag &= ~PARENB;
options.c_cflag &= ~CSTOPB;
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;

/* choosing RAW-Input */
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

/* Set the new options for the port... */
tcsetattr(fd, TCSANOW, &options);

/* endless-loop */
for (loop=0; ; loop++)
    {
    	bytes = read(fd, &buffer, sizeof(buffer));
    	printf("%4d: ", loop);
	if (bytes == -1)
	{
		perror ("read error:");
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
	bytes = read(fd, &buffer, sizeof(buffer));
	if (bytes == -1)
	{
		perror ("read error:");
	}
	else
	{
		for (x = 0; x < bytes ; x++)
		{
			c = (buffer[x] & 0xff);
			printf("%02X ",c);
		}
	printf("\n");
	}
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
