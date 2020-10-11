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

	char* logpath_index;
	char* logpath_description;
	char* logpath_volume;
	char* logpath_mute;
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

	char* logpath_cache = getenv("XDG_CACHE_HOME");

	const char* logname_index = "/bin/pulsetest.sink.index";
	pa->logpath_index = (char*) malloc(strlen(logpath_cache) + strlen(logname_index) + 1);
	strcpy(pa->logpath_index, logpath_cache);
	strcat(pa->logpath_index, logname_index);
	printf("using log file: %s\n", pa->logpath_index);

	const char* logname_description = "/bin/pulsetest.sink.description";
	pa->logpath_description = (char*) malloc(strlen(logpath_cache) + strlen(logname_description) + 1);
	strcpy(pa->logpath_description, logpath_cache);
	strcat(pa->logpath_description, logname_description);
	printf("using log file: %s\n", pa->logpath_description);

	const char* logname_volume = "/bin/pulsetest.sink.volume";
	pa->logpath_volume = (char*) malloc(strlen(logpath_cache) + strlen(logname_volume) + 1);
	strcpy(pa->logpath_volume, logpath_cache);
	strcat(pa->logpath_volume, logname_volume);
	printf("using log file: %s\n", pa->logpath_volume);

	const char* logname_mute = "/bin/pulsetest.sink.mute";
	pa->logpath_mute = (char*) malloc(strlen(logpath_cache) + strlen(logname_mute) + 1);
	strcpy(pa->logpath_mute, logpath_cache);
	strcat(pa->logpath_mute, logname_mute);
	printf("using log file: %s\n", pa->logpath_mute);
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

	free(pa->logpath_index);
	free(pa->logpath_description);
	free(pa->logpath_volume);
	free(pa->logpath_mute);

	pa->_context = NULL;
	pa->_signal = NULL;
	pa->_mainloop = NULL;
	pa->_mainloop_api = NULL;

	pa->logpath_index = NULL;
	pa->logpath_description = NULL;
	pa->logpath_volume = NULL;
	pa->logpath_mute = NULL;
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
		if (DEBUG) {
			printf("sink: %d,%s\n", i->index, i->description);
		}

		PulseAudio* pa = (PulseAudio*) userdata;

		FILE* file = fopen(pa->logpath_index, "w");
		if (file) {
			fprintf(file, "%d\n", i->index);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to %s\n", pa->logpath_index);
		}

		file = fopen(pa->logpath_description, "w");
		if (file) {
			fprintf(file, "%s\n", i->description);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to %s\n"), pa->logpath_description;
		}
	}
}

void set_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		float volume = 100.0f * (float) pa_cvolume_avg(&(i->volume)) / (float) PA_VOLUME_NORM;

		if (DEBUG) {
			printf("volume: %.0f,%d\n", volume, i->mute);
		}

		PulseAudio* pa = (PulseAudio*) userdata;

		FILE* file = fopen(pa->logpath_volume, "w");
		if (file) {
			fprintf(file, "%.0f\n", volume);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to %s\n", pa->logpath_volume);
		}

		file = fopen(pa->logpath_mute, "w");
		if (file) {
			fprintf(file, "%d\n", i->mute);
			fclose(file);
		} else {
			fprintf(stderr, "failed writing to %s\n", pa->logpath_mute);
		}
	}
}

int main(int argc, char *argv[]) {
	PulseAudio* pa = (PulseAudio*) malloc(sizeof(PulseAudio));
	initialize(pa);
	int ret = run(pa);
	destroy(pa);
	return ret;
}
