#include "utils.h"

// programming in lua: a directory iterator
// https://www.lua.org/pil/29.1.html

typedef struct dirent dirent;

int pidof(char* name, int namelen)
{
	int pid = -1;
	int readsize;
	DIR* directory = opendir("/proc");
	FILE* file;
	dirent* entry;
	char* pname = (char*) malloc(namelen + 1);
	// the size of "/proc" + "/cmdline" + "\0" is 15
	// the size of the max pid on my system is 7
	// for a total size of 22
	// cat /proc/sys/kernel/pid_max = 4194304
#define FPSIZE 22
	char* filepath = (char*) malloc(FPSIZE);

	while ((entry = readdir(directory)) != NULL) {
		pid = atoi(entry->d_name);
		if (pid == 0) continue;

		snprintf(filepath, FPSIZE, "/proc/%d/cmdline", pid);
		file = fopen(filepath, "r");
		if (file == NULL) continue;

		readsize = fread(pname, 1, namelen, file);
		fclose(file);
		if (readsize != namelen || strncmp(pname, name, namelen) != 0) continue;

		break;
	}
#undef FPSIZE

	free(pname);
	free(filepath);

	closedir(directory);
	return pid;
}

int intlen(int input) {
	if (input == 0 || input == 1) {
		return 1;
	} else {
		return ceil(log(input) / log(10));
	}
}

