#include <stdio.h>			/* Standard input/output definitions */
#include <syslog.h>			/* Syslog */
#include <string.h>			/* String function definitions */

int main( int argc, char* argv[] ) {
	char string[] = {"Ein Teststring mit Worten"};
	printf("%s\n",strchr(string, (int)'W'));
	printf("%s\n",strchr(string, (int)'T'));
	syslog(LOG_SYSLOG, "prog: bendet");
	return (0);
}
