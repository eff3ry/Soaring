#include "host_api.h"
#include <fstream>
#include <sstream>
#include <string>

// JS: log(msg)
static duk_ret_t native_log(duk_context* ctx) {
    const char* msg = duk_safe_to_string(ctx, 0);
    printf("[JS] %s\n", msg);
    return 0;
}

// JS: readFile(path)
static duk_ret_t native_read_file(duk_context* ctx) {
    const char* path = duk_require_string(ctx, 0);
    std::ifstream file(path, std::ios::in | std::ios::binary);
    if (!file) {
        duk_push_sprintf(ctx, "Error: could not open file: %s", path);
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    duk_push_lstring(ctx, ss.str().c_str(), ss.str().size());
    return 1;  // return value
}

// JS: writeFile(path, content)
static duk_ret_t native_write_file(duk_context* ctx) {
    const char* path = duk_require_string(ctx, 0);
    const char* content = duk_require_string(ctx, 1);
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (!file) {
        duk_push_sprintf(ctx, "Error: could not open file: %s", path);
        return 1;
    }
    file << content;
    if (!file) {
        duk_push_sprintf(ctx, "Error: writing to file failed: %s", path);
        return 1;
    }
    duk_push_string(ctx, "OK");
    return 1;
}

void register_host_api(duk_context* ctx) {
    duk_push_c_function(ctx, native_log, 1);
    duk_put_global_string(ctx, "log");

    duk_push_c_function(ctx, native_read_file, 1);
    duk_put_global_string(ctx, "readFile");

    duk_push_c_function(ctx, native_write_file, 2);
    duk_put_global_string(ctx, "writeFile");
}