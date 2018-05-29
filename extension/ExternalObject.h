#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <map>
#include <glib.h>

struct ExternalObjectPageData {
    guint64 page_id;
    std::map<std::string, std::string> properties;

    ExternalObjectPageData(guint64 page_id) : page_id(page_id) {}
};

class ExternalObject {

public:
    static JSClassRef get_class_def();

    static JSObjectRef create(JSContextRef ctx, ExternalObjectPageData* page);

private:
    ExternalObject() {}

    static void js_finalize(JSObjectRef obj);

    static JSValueRef js_property(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                  const JSValueRef argv[], JSValueRef* exception);

    static JSValueRef js_final_next(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                    const JSValueRef argv[], JSValueRef* exception);

    static JSValueRef js_final_back(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                    const JSValueRef argv[], JSValueRef* exception);

    ExternalObjectPageData* page;


    static JSClassRef class_def;

};