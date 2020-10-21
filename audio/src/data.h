#ifndef DATA_H
#define DATA_H

#include <pulse/pulseaudio.h>

typedef struct {
	pa_mainloop* _mainloop;
	pa_mainloop_api* _mainloop_api;
	pa_context* _context;
	pa_signal_event* _signal;

	int sink;
} PulseAudio;

#endif
