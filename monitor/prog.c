// #include <stdio.h>			/* Standard input/output definitions */
// #include <string.h>			/* String function definitions */
// #include <unistd.h>			/* UNIX standard function definitions */
// #include <fcntl.h>			/* File control definitions */
// #include <errno.h>			/* Error number definitions */
// #include <termios.h>		/* POSIX terminal control definitions */
// #include <stdlib.h>			/* converting functions */
// #include <sys/ioctl.h>		/* ioctl() */
// #include <sys/types.h>		/* ?? */
// #include <sys/stat.h>		/* ?? */
// #include <sys/time.h>		/* ?? */

// #include "types.h"			/* type definitions */
//
//
// #include<netinet/in.h>		/* web server */
// #include<sys/socket.h>		/* web server */
// #include<sys/stat.h>		/* web server */
// #include<sys/types.h>		/* web server */
// #include<unistd.h>			/* web server */




int main( int argc, char* argv[] ) {
	printf(".");
	char string[] = {"Ein Teststring mit Worten"};
	printf(".");
	printf("%s\n",strchr(string, (int)'W'));
	printf(".");
	printf("%s\n",strchr(string, (int)'T'));
	printf(".");
	return 0;
}
