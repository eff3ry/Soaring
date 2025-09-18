#include "core.h"
#include "js_engine.h"

struct Core::Impl {
    JsEngine js;
};

Core::Core() : impl(new Impl()) {}

Core::~Core() { delete impl; }

std::string Core::eval_js(const std::string& script) {
    return impl->js.eval(script);
}