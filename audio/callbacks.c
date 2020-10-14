#include "callbacks.h"

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	m->quit(m, 0);
}

void use_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		PulseAudio* pa = (PulseAudio*) userdata;

		if (pa->DEBUG) {
			printf("sink: %d,%s\n", i->index, i->description);
		}

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
		PulseAudio* pa = (PulseAudio*) userdata;

		float volume = 100.0f * (float) pa_cvolume_avg(&(i->volume)) / (float) PA_VOLUME_NORM;

		if (pa->DEBUG) {
			printf("volume: %.0f,%d\n", volume, i->mute);
		}

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

		int pid = pidof("dwmblocks", 9);
		if (pid != -1) {
			printf("signalling dwmblocks, %d\n", pid);
			int ret = kill(pid, SIGRTMIN+1);
			if (ret == 0) {
				printf("signalled dwmblocks\n");
			} else {
				fprintf(stderr, "failed to signal dwmblocks, %d\n", ret);
			}
		}
	}
}

void select_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudio* pa = (PulseAudio*) userdata;
	if (i) {
		pa->sink = i->index;
		if (pa->DEBUG) {
			printf("iterating over sink %d\n", pa->sink);
		}
	}
	if (eol) {
		if (pa->DEBUG) {
			printf("ending with sink %d\n", pa->sink);
		}
		pa_context_get_sink_info_by_index(c, pa->sink, use_sink_callback, userdata);
	}
}


void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata) {
	unsigned eventtype = type & PA_SUBSCRIPTION_EVENT_TYPE_MASK;
	PulseAudio* pa = (PulseAudio*) userdata;

	if (eventtype == PA_SUBSCRIPTION_EVENT_NEW) {
		if (pa->DEBUG) {
			printf("new sink\n");
		}
		pa_context_get_sink_info_by_index(c, idx, use_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_REMOVE) {
		if (pa->DEBUG) {
			printf("removed sink\n");
		}
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_CHANGE) {
		if (pa->DEBUG) {
			printf("changed volume\n");
		}
		pa_context_get_sink_info_by_index(c, idx, set_volume_callback, userdata);
	}
}



void initialize_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudio* pa = (PulseAudio*) userdata;
	if (i) {
		pa->sink = i->index;
		if (pa->DEBUG) {
			printf("iterating over sink %d\n", pa->sink);
		}
	}
	if (eol) {
		if (pa->DEBUG) {
			printf("ending with sink %d\n", pa->sink);
		}
		pa_context_get_sink_info_by_index(c, pa->sink, set_volume_callback, userdata);
		pa_context_get_sink_info_by_index(c, pa->sink, use_sink_callback, userdata);
	}
}


void context_state_callback(pa_context *c, void *userdata) {
	if (pa_context_get_state(c) == PA_CONTEXT_READY) {
		pa_context_get_sink_info_list(c, initialize_sink_callback, userdata);
		pa_context_set_subscribe_callback(c, subscribe_callback, userdata);
		pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, NULL);
	}
}

