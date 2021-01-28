#include <pulse/pulseaudio.h>
#include "callbacks.h"
#include <string.h>
#include <stdio.h>
#include "utils.h"
#include <math.h>
#include "data.h"

/*
 * quit mainloop
 */
void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata) {
	m->quit(m, 0);
}

/*
 * update the information on the given sink
 */
void write_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		int pid;
		int ret;
		int digits = intlen(i->index);
		int volume = round(100.0f * (float) pa_cvolume_avg(&(i->volume)) / (float) PA_VOLUME_NORM);
		int logpath_description_length;
		int logpath_volume_length;
		int logpath_mute_length;
		char* logpath_description;
		char* logpath_volume;
		char* logpath_mute;
		char* logpath_cache = getenv("XDG_CACHE_HOME");
		FILE* file;

		logpath_description_length = strlen(logpath_cache)
			+ strlen("/bin/pulsetest.sinks.") + digits + strlen(".description") + 1;
		logpath_description = (char*) malloc(logpath_description_length);
		snprintf(logpath_description, logpath_description_length, "%s/bin/pulsetest.sinks.%d.description", logpath_cache, i->index);
		printf("[write_sink]\t%s\t<- %s\n", logpath_description, i->description);
		file = fopen(logpath_description, "w");
		if (file) {
			fprintf(file, "%s\n", i->description);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", logpath_description);
		}

		logpath_volume_length = strlen(logpath_cache)
			+ strlen("/bin/pulsetest.sinks.") + digits + strlen(".volume") + 1;
		logpath_volume = (char*) malloc(logpath_volume_length);
		snprintf(logpath_volume, logpath_volume_length, "%s/bin/pulsetest.sinks.%d.volume", logpath_cache, i->index);
		printf("[write_sink]\t%s\t<- %d\n", logpath_volume, volume);
		file = fopen(logpath_volume, "w");
		if (file) {
			fprintf(file, "%d\n", volume);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", logpath_volume);
		}

		logpath_mute_length = strlen(logpath_cache)
			+ strlen("/bin/pulsetest.sinks.") + digits + strlen(".mute") + 1;
		logpath_mute = (char*) malloc(logpath_mute_length);
		snprintf(logpath_mute, logpath_mute_length, "%s/bin/pulsetest.sinks.%d.mute", logpath_cache, i->index);
		printf("[write_sink]\t%s\t\t<- %d\n", logpath_mute, i->mute);
		file = fopen(logpath_mute, "w");
		if (file) {
			fprintf(file, "%d\n", i->mute);
			fclose(file);
		} else {
			fprintf(stderr, "[write_sink]\tfailed writing to %s\n", logpath_mute);
		}

		pid = pidof("dwmblocks", 9);
		if (pid != -1) {
			printf("[write_sink]\tsignalling dwmblocks (%d)\n", pid);
			ret = kill(pid, SIGRTMIN+1);
			if (ret != 0) {
				fprintf(stderr, "[write_sink]\tfailed to signal dwmblocks (%d)\n", ret);
			}
		}

		free(logpath_description);
		free(logpath_volume);
		free(logpath_mute);
	}
}

/*
 * update the reference to the newest sink
 */
void write_newest_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		int logpath_newest_sink_length;
		char* logpath_newest_sink;
		char* logpath_cache = getenv("XDG_CACHE_HOME");
		FILE* file;

		logpath_newest_sink_length = strlen(logpath_cache) + strlen("/bin/pulsetest.newest_sink_index") + 1;
		logpath_newest_sink = (char*) malloc(logpath_newest_sink_length);
		snprintf(logpath_newest_sink, logpath_newest_sink_length, "%s/bin/pulsetest.newest_sink_index", logpath_cache);
		printf("[write_newest]\t%s\t<- %d\n", logpath_newest_sink, i->index);

		file = fopen(logpath_newest_sink, "w");
		if (file) {
			fprintf(file, "%d\n", i->index);
			fclose(file);
		} else {
			fprintf(stderr, "[write_newest]\tfailed writing to %s\n", logpath_newest_sink);
		}

		free(logpath_newest_sink);
	}
}

// TODO: find a better place for this
#define NOTIFICATION_ID 1005

/*
 * create a desktop notification for the given sink
 */
void notify_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	if (i) {
		int length;
		char* command;

		// TODO: determine if strlen is safe to use
		length = intlen(NOTIFICATION_ID) + strlen(i->description) + strlen("dunstify -r '' 'New sink' ''") + 1;
		command = (char*) malloc(length);
		snprintf(command, length, "dunstify -r '%d' 'New sink' '%s'", NOTIFICATION_ID, i->description);

		printf("[notify]\texecuting %s\n", command);
		if (system(command) != 0) {
			fprintf(stderr, "[notify]\tfailed executing notify command\n");
		}
	}
}

#undef NOTIFICATION_ID

/*
 * log success or failure of switching a sink input
 */
void moved_sink_input_callback(pa_context *c, int status, void *userdata) {
	if (status) {
		printf("[moved_sink_input]\tmoved sink\n");
	} else {
		fprintf(stderr, "[moved_sink_input]\tfailed moving sink\n");
	}
}

/*
 * switch all sink inputs to the newest sink, which is indicated by pa->sink
 */
void switch_sink_inputs_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata) {
	if (i) {
		PulseAudio* pa = (PulseAudio*) userdata;
		printf("[switch_sink_inputs]\tmoving %s, sink input %d -> sink %d\n", i->name, i->index, pa->sink);
		pa_context_move_sink_input_by_index(c, i->index, pa->sink, moved_sink_input_callback, userdata);
	}
}

/*
 * loop over all sinks, the last sink is the newest
 */
void select_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata) {
	PulseAudio* pa = (PulseAudio*) userdata;
	if (i) {
		pa->sink = i->index;
		printf("[select_sink]\titerating over %d\n", pa->sink);
	}
	// eol means last sink
	if (eol) {
		printf("[select_sink]\tselecting %d\n", pa->sink);
		pa_context_get_sink_info_by_index(c, pa->sink, write_sink_callback, userdata);
		pa_context_get_sink_info_by_index(c, pa->sink, write_newest_sink_callback, userdata);
		pa_context_get_sink_info_by_index(c, pa->sink, notify_sink_callback, userdata);
		pa_context_get_sink_input_info_list(c, switch_sink_inputs_callback, userdata);
	}
}

/*
 * handle all types of sink events
 */
void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata) {
	unsigned eventtype = type & PA_SUBSCRIPTION_EVENT_TYPE_MASK;

	// if a new sink is created, use it
	if (eventtype == PA_SUBSCRIPTION_EVENT_NEW) {
		PulseAudio* pa = (PulseAudio*) userdata;
		pa->sink = idx;
		printf("[subscribe]\tnew sink %d\n", idx);
		pa_context_get_sink_info_by_index(c, idx, write_sink_callback, userdata);
		pa_context_get_sink_info_by_index(c, idx, write_newest_sink_callback, userdata);
		pa_context_get_sink_info_by_index(c, idx, notify_sink_callback, userdata);
		pa_context_get_sink_input_info_list(c, switch_sink_inputs_callback, userdata);
	}

	// if a sink is removed, find the newest sink
	// TODO: find newest sink only if removed sink is the newest sink
	if (eventtype == PA_SUBSCRIPTION_EVENT_REMOVE) {
		printf("[subscribe]\tremoved sink %d\n", idx);
		// TODO: delete information of removed sink
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
	}

	if (eventtype == PA_SUBSCRIPTION_EVENT_CHANGE) {
		printf("[subscribe]\tchanged volume of %d\n", idx);
		// TODO: create new callback to only update the volume file
		pa_context_get_sink_info_by_index(c, idx, write_sink_callback, userdata);
	}
}

/*
 * entry point into this application
 */
void context_state_callback(pa_context *c, void *userdata) {
	if (pa_context_get_state(c) == PA_CONTEXT_READY) {
		printf("[context_state]\tready\n");
		// select an initial sink
		pa_context_get_sink_info_list(c, select_sink_callback, userdata);
		// subscribe to sink events
		pa_context_set_subscribe_callback(c, subscribe_callback, userdata);
		pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, NULL);
	}
}
