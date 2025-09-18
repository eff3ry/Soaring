#pragma once

#include <string>

class Core {
public:
    Core();
    ~Core();

    // Runs a JS script, returns the result as a string
    std::string eval_js(const std::string& script);

    // (Future) expose more methods for module loading, host API, etc.

private:
    struct Impl;
    Impl* impl;
};