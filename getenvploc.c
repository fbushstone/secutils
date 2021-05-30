/*get environment variable (portable) location*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *location;

	if(argc != 3) {
		printf("usage ./getenvploc <variable name> <program name>");
		exit(1);
	}

	/*Add the length of name of this program for the location in an ideal environment (a program whose name is 0 bytes long) and then add the length of the name of the program we want to exploit.*/
	location = getenv(argv[1]) + strlen(argv[0]) - strlen(argv[2]);

	printf("%s is located at %p.\n", argv[1], location);

	return 0;
}
