#include <pulse/pulseaudio.h>
#include <assert.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <vector>

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata);
void context_state_callback(pa_context *c, void *userdata);
void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata);
void initialize_sinks_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void use_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void set_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);

class PulseAudio
{
	private:
		pa_mainloop* _mainloop;
		pa_mainloop_api* _mainloop_api;
		pa_context* _context;
		pa_signal_event* _signal;

	public:
		PulseAudio(): _mainloop(NULL), _mainloop_api(NULL), _context(NULL), _signal(NULL) {
		}

		bool initialize() {
			_mainloop = pa_mainloop_new();
			_mainloop_api = pa_mainloop_get_api(_mainloop);
			pa_signal_init(_mainloop_api);
			_signal = pa_signal_new(SIGINT, exit_signal_callback, this);
			signal(SIGPIPE, SIG_IGN);
			_context = pa_context_new(_mainloop_api, "PulseAudio Test");
			pa_context_connect(_context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
			pa_context_set_state_callback(_context, context_state_callback, this);
			return true;
		}

		int run() {
			int ret = 1;
			pa_mainloop_run(_mainloop, &ret);
			return ret;
		}

		void quit(int ret = 0) {
			_mainloop_api->quit(_mainloop_api, ret);
		}

		void destroy() {
			if (_context) {
				pa_context_unref(_context);
				_context = NULL;
			}

			if (_signal) {
				pa_signal_free(_signal);
				pa_signal_done();
				_signal = NULL;
			}

			if (_mainloop) {
				pa_mainloop_free(_mainloop);
				_mainloop = NULL;
				_mainloop_api = NULL;
			}
		}

		~PulseAudio() {
			destroy();
		}
};

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	((PulseAudio*) userdata)->quit(0);
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
	PulseAudio pa = PulseAudio();
	pa.initialize();
	return pa.run();
}
