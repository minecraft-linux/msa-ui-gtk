#include "JSUtils.h"

std::string JSUtils::get_js_string(JSStringRef ref) {
    std::string str;
    str.resize(JSStringGetMaximumUTF8CStringSize(ref));
    size_t size = JSStringGetUTF8CString(ref, &str[0], str.size());
    str.resize(size);
    return str;
}

std::string JSUtils::get_js_string(JSContextRef ctx, JSValueRef ref) {
    JSStringRef str = JSValueToStringCopy(ctx, ref, nullptr);
    std::string ret = get_js_string(str);
    JSStringRelease(str);
    return ret;
}

JSStringRef JSUtils::new_js_string(JSContextRef ctx, std::string const& str) {
    return JSStringCreateWithUTF8CString(str.data());
}

JSValueRef JSUtils::new_js_string_value(JSContextRef ctx, std::string const& str) {
    auto jstr = new_js_string(ctx, str);
    auto val = JSValueMakeString(ctx, jstr);
    JSStringRelease(jstr);
    return val;
}