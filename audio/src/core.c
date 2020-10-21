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
