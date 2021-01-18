#include "data.h"
#include "core.h"

int main(int argc, char *argv[]) {
	int ret;
	PulseAudio* pa;

	pa = (PulseAudio*) malloc(sizeof(PulseAudio));
	initialize(pa);
	ret = run(pa);

	destroy(pa);
	free(pa);
	pa = NULL;

	return ret;
}
