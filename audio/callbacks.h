#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "data.h"
#include "pidof.h"

void exit_signal_callback(pa_mainloop_api *m, pa_signal_event *e, int sig, void *userdata);
void context_state_callback(pa_context *c, void *userdata);
void subscribe_callback(pa_context *c, pa_subscription_event_type_t type, uint32_t idx, void *userdata);
void initialize_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void select_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void use_sink_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
void set_volume_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);

#endif
