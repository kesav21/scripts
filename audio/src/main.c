#include "core.h"

int main(int argc, char *argv[]) {
	PulseAudio* pa = (PulseAudio*) malloc(sizeof(PulseAudio));
	initialize(pa);
	int ret = run(pa);
	destroy(pa);
	free(pa);
	pa = NULL;
	return ret;
}
