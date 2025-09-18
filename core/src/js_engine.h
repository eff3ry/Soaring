#pragma once

#include <string>
#include "duktape.h"

class JsEngine {
public:
    JsEngine();
    ~JsEngine();

    // Evaluate JS code and return result as string
    std::string eval(const std::string& script);

private:
    duk_context* ctx;
};