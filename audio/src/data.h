#ifndef DATA_H
#define DATA_H

#include <pulse/pulseaudio.h>

typedef struct {
	pa_mainloop* _mainloop;
	pa_mainloop_api* _mainloop_api;
	pa_context* _context;
	pa_signal_event* _signal;

	char* logpath_index;
	char* logpath_description;
	char* logpath_volume;
	char* logpath_mute;

	int sink;
	int DEBUG;
} PulseAudio;

#endif
