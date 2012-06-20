#include <stdio.h>
#include <stdlib.h>

static const char* USAGE = "Usage: %s pid\n";
static const char* INVALID_PID = "Invalid pid: %s\n";

int main(int argc, char **argv) {
	// Figure out the PID the user's trying to get info about
	if (argc != 2) {
		printf(USAGE, argv[0]);
		return 1;
	}

	int pid = atoi(argv[1]);
	if (pid == 0) {
		printf(INVALID_PID, argv[1]);
		return 1;
	}

	printf("I would get information about %d\n", pid);

	return 0;
}