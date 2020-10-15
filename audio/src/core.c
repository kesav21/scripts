#include "core.h"

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
	printf("[initialize]\tusing %s\n", pa->logpath_index);

	const char* logname_description = "/bin/pulsetest.sink.description";
	pa->logpath_description = (char*) malloc(strlen(logpath_cache) + strlen(logname_description) + 1);
	strcpy(pa->logpath_description, logpath_cache);
	strcat(pa->logpath_description, logname_description);
	printf("[initialize]\tusing %s\n", pa->logpath_description);

	const char* logname_volume = "/bin/pulsetest.sink.volume";
	pa->logpath_volume = (char*) malloc(strlen(logpath_cache) + strlen(logname_volume) + 1);
	strcpy(pa->logpath_volume, logpath_cache);
	strcat(pa->logpath_volume, logname_volume);
	printf("[initialize]\tusing %s\n", pa->logpath_volume);

	const char* logname_mute = "/bin/pulsetest.sink.mute";
	pa->logpath_mute = (char*) malloc(strlen(logpath_cache) + strlen(logname_mute) + 1);
	strcpy(pa->logpath_mute, logpath_cache);
	strcat(pa->logpath_mute, logname_mute);
	printf("[initialize]\tusing %s\n", pa->logpath_mute);
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
