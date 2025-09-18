#include "js_engine.h"
#include "host_api.h" 
#include <stdexcept>

JsEngine::JsEngine() {
    ctx = duk_create_heap_default();
    if (!ctx) throw std::runtime_error("Failed to create Duktape context");
    register_host_api(ctx); // <-- Register native APIs here
}

JsEngine::~JsEngine() {
    if (ctx) duk_destroy_heap(ctx);
}

std::string JsEngine::eval(const std::string& script) {
    if (duk_peval_string(ctx, script.c_str()) != 0) {
        std::string err = duk_safe_to_string(ctx, -1);
        duk_pop(ctx);
        return "Error: " + err;
    }
    std::string result = duk_safe_to_string(ctx, -1);
    duk_pop(ctx);
    return result;
}