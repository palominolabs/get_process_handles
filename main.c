#include <stdio.h>
#include <stdlib.h>
#include <libproc.h>
#include <sys/proc_info.h>


static const char* USAGE = "Usage: %s pid\n";
static const char* INVALID_PID = "Invalid pid: %s\n";
static const char* UNABLE_TO_GET_PROC_FDS = "Unable to get open file handles for %d\n";
static const char* OUT_OF_MEMORY = "Out of memory. Unable to allocate buffer with %d bytes\n";
static const char* OPEN_FILE = "Open file: %s\n";

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

	// Figure out the size of the buffer needed to hold the list of open FDs
	int bufferSize = proc_pidinfo(pid, PROC_PIDLISTFDS, 0, 0, 0);
	if (bufferSize == -1) {
		printf(UNABLE_TO_GET_PROC_FDS, pid);
		return 1;
	}

	// Get the list of open FDs
	struct proc_fdinfo *procFDInfo = (struct proc_fdinfo *)malloc(bufferSize);
	if (!procFDInfo) {
		printf(OUT_OF_MEMORY, bufferSize);
		return 1;
	}
	proc_pidinfo(pid, PROC_PIDLISTFDS, 0, procFDInfo, bufferSize);
	int numberOfProcFDs = bufferSize / PROC_PIDLISTFD_SIZE;

	int i;
	for(i = 0; i < numberOfProcFDs; i++) {
		if(procFDInfo[i].proc_fdtype == PROX_FDTYPE_VNODE) {
			struct vnode_fdinfowithpath vnodeInfo;
			int bytesUsed = proc_pidfdinfo(pid, procFDInfo[i].proc_fd, PROC_PIDFDVNODEPATHINFO, &vnodeInfo, PROC_PIDFDVNODEPATHINFO_SIZE);
			if (bytesUsed == PROC_PIDFDVNODEPATHINFO_SIZE) {
				printf(OPEN_FILE, vnodeInfo.pvip.vip_path);
			}
		}
	}


	

	return 0;
}