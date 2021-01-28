#include "callbacks.h"
#include "data.h"

int main(int argc, char *argv[]) {
	int ret;
	pa_mainloop* mainloop;
	pa_mainloop_api* mainloop_api;
	pa_context* context;
	pa_signal_event* my_signal;
	PulseAudio* pa;

	pa = (PulseAudio*) malloc(sizeof(PulseAudio));

	mainloop = pa_mainloop_new();
	mainloop_api = pa_mainloop_get_api(mainloop);

	pa_signal_init(mainloop_api);
	my_signal = pa_signal_new(SIGINT, exit_signal_callback, pa);
	signal(SIGPIPE, SIG_IGN);

	context = pa_context_new(mainloop_api, "PulseAudio Test");
	pa_context_connect(context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);
	pa_context_set_state_callback(context, context_state_callback, pa);

	pa_mainloop_run(mainloop, &ret);

	pa_context_unref(context);
	pa_signal_free(my_signal);
	pa_signal_done();
	pa_mainloop_free(mainloop);
	free(pa);

	return ret;
}
