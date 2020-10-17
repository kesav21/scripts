#include "callbacks.h"

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	m->quit(m, 0);
}

void write_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		int pid, ret;
		FILE* file;
		PulseAudio* pa = (PulseAudio*) userdata;
		float volume = 100.0f * (float) pa_cvolume_avg(&(i->volume)) / (float) PA_VOLUME_NORM;

		printf("[write_sink]\t%d \"%s\" %.0f %d\n", i->index, i->description, volume, i->mute);

		file = fopen(pa->logpath_index, "w");
		if (file) {
			fprintf(file, "%d\n", i->index);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", pa->logpath_index);
		}

		file = fopen(pa->logpath_description, "w");
		if (file) {
			fprintf(file, "%s\n", i->description);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", pa->logpath_description);
		}

		file = fopen(pa->logpath_volume, "w");
		if (file) {
			fprintf(file, "%.0f\n", volume);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", pa->logpath_volume);
		}

		file = fopen(pa->logpath_mute, "w");
		if (file) {
			fprintf(file, "%d\n", i->mute);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", pa->logpath_mute);
		}

		pid = pidof("dwmblocks", 9);
		if (pid != -1) {
			printf("[write_sink]\tsignalling dwmblocks (%d)\n", pid);
			ret = kill(pid, SIGRTMIN+1);
			if (ret != 0) {
				fprintf(stderr, "[write_sink]\tfailed to signal dwmblocks (%d)\n", ret);
			}
		}
	}
}

void select_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudio* pa = (PulseAudio*) userdata;
	if (i) {
		pa->sink = i->index;
		printf("[select_sink]\titerating over %d\n", pa->sink);
	}
	if (eol) {
		printf("[select_sink]\tselecting %d\n", pa->sink);
		pa_context_get_sink_info_by_index(c, pa->sink, write_sink_callback, userdata);
	}
}


void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata) {
	unsigned eventtype = type & PA_SUBSCRIPTION_EVENT_TYPE_MASK;

	if (eventtype == PA_SUBSCRIPTION_EVENT_NEW) {
		printf("[subscribe]\tnew sink %d\n", idx);
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_REMOVE) {
		printf("[subscribe]\tremoved sink %d\n", idx);
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_CHANGE) {
		printf("[subscribe]\tchanged volume of %d\n", idx);
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
	}
}

void context_state_callback(pa_context *c, void *userdata) {
	PulseAudio* pa = (PulseAudio*) userdata;
	if (pa_context_get_state(c) == PA_CONTEXT_READY) {
		printf("[context_state]\tready\n");
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
		pa_context_set_subscribe_callback(c, subscribe_callback, userdata);
		pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, NULL);
	}
}

