#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h> /* converting funktions - atoi()  */

int main( int argc, char* argv[] ) {

   int driveID=12;
   int commandID=0;
   char * serialPort="/dev/ttyUSB0";
   char serialPort2[]="/dev/ttyUSB0";
   if (argc > 1) {
   	driveID=atoi(argv[1]);
   }
   
   if (argc > 2) {
   if (strcmp(argv[2], "stop") == 0) commandID=0;
   if (strcmp(argv[2], "up") == 0) commandID=1;
   if (strcmp(argv[2], "hoch") == 0) commandID=1;
   if (strcmp(argv[2], "down") == 0) commandID=2;
   if (strcmp(argv[2], "runter") == 0) commandID=2;
   }
   
   if (argc > 3) {
   	serialPort=argv[3];
   } else {
   	serialPort=serialPort2;
   }
/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */

  int fd; /* File descriptor for the port */


  fd = open(serialPort, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /*
    * Could not open the port.
    */

    perror("open_port: Unable to open serialPort ");
  }
else
    fcntl(fd, F_SETFL, 0);


struct termios options;

/*
 * Get the current options for the port...
 */

tcgetattr(fd, &options);

/*
 * Set the baud rates to 2400...
 */

cfsetispeed(&options, B2400);
cfsetospeed(&options, B2400);

/*
 * Enable the receiver and set local mode...
 */

options.c_cflag |= (CLOCAL | CREAD);

/*
 * Setting Character Size
 */
options.c_cflag &= ~CSIZE; /* Mask the character size bits */
options.c_cflag |= CS8;    /* Select 8 data bits */


/*
 * Setting 8N1
 */
options.c_cflag &= ~PARENB;
options.c_cflag &= ~CSTOPB;
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;

/*
 * choosing RAW-Input
 */
options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

/*
 * Set the new options for the port...
 */

tcsetattr(fd, TCSANOW, &options);


/*
 * Write to port
 * example:
 * Motor 12 up	n = write(fd, "\x5c\x01\xA3", 4);
 * Drive 12 down	n = write(fd, "\x5c\x02\xA2", 4);
 */
	int n;
    char tmpBytes[4];
    tmpBytes[0]=0x50 + driveID;
    tmpBytes[1]=commandID;
    tmpBytes[2]=tmpBytes[0] + tmpBytes[1];
    tmpBytes[2]=~(tmpBytes[0] + tmpBytes[1])+ 1;
//    n = write(fd, "\x5c\x02\xA2", 4);
    n = write(fd, tmpBytes, 3);
    if (n < 0)
      fputs("write() of smi command failed!\n", stderr);
      printf("-> 0x%02X 0x%02X 0x%02X\n",tmpBytes[0],tmpBytes[1],tmpBytes[2]);


/*
 * Read from port
 */
// fcntl(fd, F_SETFL, FNDELAY);
// fcntl(fd, F_SETFL, 0);


/*
 * Close Port
 */

   close(fd);
   return (0);
}
