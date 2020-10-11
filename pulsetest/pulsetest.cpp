#include <pulse/pulseaudio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <vector>

typedef struct {
	pa_mainloop* _mainloop;
	pa_mainloop_api* _mainloop_api;
	pa_context* _context;
	pa_signal_event* _signal;
} PulseAudio;

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata);
void context_state_callback(pa_context *c, void *userdata);
void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata);
void initialize_sinks_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void use_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void set_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);


void initialize(PulseAudio* pa) {
	pa->_mainloop = pa_mainloop_new();
	pa->_mainloop_api = pa_mainloop_get_api(pa->_mainloop);

	pa_signal_init(pa->_mainloop_api);
	pa->_signal = pa_signal_new(SIGINT, exit_signal_callback, pa);
	signal(SIGPIPE, SIG_IGN);

	pa->_context = pa_context_new(pa->_mainloop_api, "PulseAudio Test");
	pa_context_connect(pa->_context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
	pa_context_set_state_callback(pa->_context, context_state_callback, pa);
}

int run(PulseAudio* pa) {
	int ret = 1;
	pa_mainloop_run(pa->_mainloop, &ret);
	return ret;
}

void destroy(PulseAudio* pa) {
	pa_context_unref(pa->_context);
	pa_signal_free(pa->_signal);
	pa_signal_done();
	pa_mainloop_free(pa->_mainloop);

	pa->_context = NULL;
	pa->_signal = NULL;
	pa->_mainloop = NULL;
	pa->_mainloop_api = NULL;
}

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	m->quit(m, 0);
}

void context_state_callback(pa_context *c, void *userdata) {
	if (pa_context_get_state(c) == PA_CONTEXT_READY) {
		pa_context_get_sink_info_list(c, initialize_sinks_callback, userdata);
		pa_context_set_subscribe_callback(c, subscribe_callback, userdata);
		pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, NULL);
	}
}

std::vector<int> sinks;
#define DEBUG 0

void initialize_sinks_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		sinks.push_back(i->index);

		if (DEBUG) {
			printf("{");
			for (int j: sinks) printf("%d,", j);
			printf("}\n");
		}
	}
	if (eol) {
		pa_context_get_sink_info_by_index(c, sinks.back(), set_volume_callback, userdata);
		pa_context_get_sink_info_by_index(c, sinks.back(), use_sink_callback, userdata);
	}
}

void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata) {
	unsigned eventtype = type & PA_SUBSCRIPTION_EVENT_TYPE_MASK;

	if (eventtype == PA_SUBSCRIPTION_EVENT_NEW) {
		sinks.push_back(idx);

		if (DEBUG) {
			printf("new sink\n");
			printf("{");
			for (int j: sinks) printf("%d,", j);
			printf("}\n");
		}

		pa_context_get_sink_info_by_index(c, idx, use_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_REMOVE) {
		sinks.pop_back();

		if (DEBUG) {
			printf("removed sink\n");
			printf("{");
			for (int j: sinks) printf("%d,", j);
			printf("}\n");
		}

		pa_context_get_sink_info_by_index(c, sinks.back(), use_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_CHANGE) {
		if (DEBUG) {
			printf("changed volume\n");
		}
		pa_context_get_sink_info_by_index(c, idx, set_volume_callback, userdata);
	}
}

void use_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		char sinklog[100] = "";
		strcpy(sinklog, getenv("XDG_CACHE_HOME"));
		strcat(sinklog, "/bin/pulsetest.sink");

		if (DEBUG) {
			printf("sink: %d,%s\n", i->index, i->description);
			printf("writing to %s\n", sinklog);
		}

		FILE *file = fopen(sinklog, "w");
		if (file) {
			fprintf(file, "%d,%s\n", i->index, i->description);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to sinklog\n");
		}
	}
}

void set_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		float volume = 100.0f * (float) pa_cvolume_avg(&(i->volume)) / (float) PA_VOLUME_NORM;

		char volumelog[100] = "";
		strcpy(volumelog, getenv("XDG_CACHE_HOME"));
		strcat(volumelog, "/bin/pulsetest.volume");

		if (DEBUG) {
			printf("volume: %.0f,%d\n", volume, i->mute);
			printf("writing to %s\n", volumelog);
		}

		FILE *file = fopen(volumelog, "w");
		if (file) {
			fprintf(file, "%.0f,%d\n", volume, i->mute);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to volumelog\n");
		}
	}
}

int main(int argc, char *argv[]) {
	PulseAudio* pa = (PulseAudio*) malloc(sizeof(PulseAudio));
	initialize(pa);
	int ret =  run(pa);
	destroy(pa);
	return ret;
}
