#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <map>

class ExternalObject {

public:
    static JSClassRef get_class_def();

private:
    static JSValueRef js_property(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                  const JSValueRef argv[], JSValueRef* exception);

    static JSValueRef js_final_next(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                    const JSValueRef argv[], JSValueRef* exception);

    static JSValueRef js_final_back(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                    const JSValueRef argv[], JSValueRef* exception);

    static std::map<std::string, std::string> properties;


    static JSClassRef class_def;

};