#pragma once

#include <JavaScriptCore/JSStringRef.h>
#include <string>

class JSUtils {

public:
    static std::string get_js_string(JSStringRef ref);

    static std::string get_js_string(JSContextRef ctx, JSValueRef ref);

    static JSStringRef new_js_string(JSContextRef ctx, std::string const& str);

    static JSValueRef new_js_string_value(JSContextRef ctx, std::string const& str);

};