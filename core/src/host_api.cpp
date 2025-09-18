#include "host_api.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <iostream>
#include <curl/curl.h>

// log(msg)
static duk_ret_t native_log(duk_context* ctx) {
    const char* msg = duk_safe_to_string(ctx, 0);
    printf("[JS] %s\n", msg);
    return 0;
}
// error(msg)
static duk_ret_t native_error(duk_context* ctx) {
    const char* msg = duk_safe_to_string(ctx, 0);
    fprintf(stderr, "[JS ERROR] %s\n", msg);
    return 0;
}

// readFile(path)
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
    return 1;
}

// writeFile(path, content)
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

// Helper for cURL write callback
static size_t curl_write_string(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* str = reinterpret_cast<std::string*>(userdata);
    str->append(reinterpret_cast<char*>(ptr), size * nmemb);
    return size * nmemb;
}

// fetch(url)
static duk_ret_t native_fetch(duk_context* ctx) {
    const char* url = duk_require_string(ctx, 0);
    std::string result;
    CURL* curl = curl_easy_init();
    if (!curl) {
        duk_push_string(ctx, "Error: curl_easy_init failed");
        return 1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_string);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        duk_push_sprintf(ctx, "Error: %s", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);
    duk_push_lstring(ctx, result.c_str(), result.size());
    return 1;
}

void register_host_api(duk_context* ctx) {
    // log and error as global functions
    duk_push_c_function(ctx, native_log, 1);
    duk_put_global_string(ctx, "log");
    duk_push_c_function(ctx, native_error, 1);
    duk_put_global_string(ctx, "error");

    // Add console object with log and error
    duk_push_object(ctx);
    duk_push_c_function(ctx, native_log, 1);
    duk_put_prop_string(ctx, -2, "log");
    duk_push_c_function(ctx, native_error, 1);
    duk_put_prop_string(ctx, -2, "error");
    duk_put_global_string(ctx, "console");

    duk_push_c_function(ctx, native_read_file, 1);
    duk_put_global_string(ctx, "readFile");
    duk_push_c_function(ctx, native_write_file, 2);
    duk_put_global_string(ctx, "writeFile");
    duk_push_c_function(ctx, native_fetch, 1);
    duk_put_global_string(ctx, "fetch");
}