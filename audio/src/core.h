#ifndef CORE_H
#define CORE_H

#include "data.h"

void initialize(PulseAudio* pa);
int run(PulseAudio* pa);
void destroy(PulseAudio* pa);

#endif
