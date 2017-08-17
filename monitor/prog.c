#include <stdio.h>			/* Standard input/output definitions */
#include <str.ing.h>			/* String function definitions */

int main( int argc, char* argv[] ) {
	printf(".");
	char string[] = {"Ein Teststring mit Worten"};
	printf(".");
	printf("%s\n",strchr(string, (int)'W'));
	printf(".");
	printf("%s\n",strchr(string, (int)'T'));
	printf(".");
	return (0);
}
