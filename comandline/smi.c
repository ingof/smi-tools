#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting functions */

int main( int argc, char* argv[] ) {
	/* default for commandline parameter */
	int driveID=12;
	int commandID=0;
	char * serialPort="/dev/ttyUSB1";
	char serialPort2[]="/dev/ttyUSB1";
	int serialWait=60;
	int fd; /* File descriptor for the port */
	int x;
	int bytes;
	char c;
	char buffer[12];
   //char *bufptr;
    
   /* dirst paramter is driveID */
   if (argc > 1) {
   	driveID=atoi(argv[1]);
   }
   /*secon paramter is the command */
   if (argc > 2)
   {
   	/* Stop command */
   	if (strcmp(argv[2], "stop") == 0) commandID=0;
	/* up-command */
 	if (strcmp(argv[2], "up") == 0) commandID=1;
 	if (strcmp(argv[2], "hoch") == 0) commandID=1;
	/* down command */
	if (strcmp(argv[2], "down") == 0) commandID=2;
	if (strcmp(argv[2], "runter") == 0) commandID=2;
   }
   
   /* third parameter is serialPort*/
   if (argc > 3)
   {
   	serialPort=argv[3];
   } else {
   	serialPort=serialPort2;
   	
   }
   /* fourth parameter is serialWait*/
   if (argc > 4)
   {
   	serialWait=atoi(argv[4]);
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
  	perror("open_port: Unable to open serialPort ");
  } 
  else
  {
  	fcntl(fd, F_SETFL, 0);
  	
  }


struct termios options;

/* Get the current options for the port... */
tcgetattr(fd, &options);

/* Set the baud rates to 2400... */
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
tcsetattr(fd, TCSANOW, &options);


/* Write to port */
	int n;
	char tmpBytes[4];
	tmpBytes[0]=0x50 + driveID;
	tmpBytes[1]=commandID;
	tmpBytes[2]=tmpBytes[0] + tmpBytes[1];
	tmpBytes[2]=~(tmpBytes[0] + tmpBytes[1])+ 1;
    n = write(fd, tmpBytes, 3);
    if (n < 0)
      fputs("write() of smi command failed!\n", stderr);
      printf("%02X %02X %02X: ",tmpBytes[0],tmpBytes[1],tmpBytes[2]);

	/* wait 60ms for answer */
	usleep(serialWait*1000);

	/* Read from port */
	fcntl(fd, F_SETFL, FNDELAY);
	bytes = read(fd, &buffer, sizeof(buffer));
//    printf("number of bytes read is %d\n", bytes);
//    printf("->%02X.\n", buffer);
    //perror ("read error:");
    for (x = 3; x < bytes ; x++) {
        c = buffer[x];
        printf("%02X ",c);
    }
    printf("\n");

/* Close Port */
   close(fd);
   printf("return.%03d.",buffer[bytes]);
   if (buffer[bytes]==0xff)
   	{
   		return (0);
   	}
   	else
   	{
   		return(-1);
   	}
   }

}


int open_port(void)
{
	int fd; /* File descriptor for the port */
	fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		/* Could not open the port. */
		perror("open_port: Unable to open /dev/ttyUSB1 - ");
		
	}
	else
	{
		fcntl(fd, F_SETFL, FNDELAY);
	}
	return (fd);
}
