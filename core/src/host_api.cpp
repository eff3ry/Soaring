#include "host_api.h"

// Example: register a simple log() function for JS
static duk_ret_t native_log(duk_context* ctx) {
    const char* msg = duk_safe_to_string(ctx, 0);
    printf("[JS log]: %s\n", msg);
    return 0; // no return value
}

void register_host_api(duk_context* ctx) {
    duk_push_c_function(ctx, native_log, 1);
    duk_put_global_string(ctx, "log");
}