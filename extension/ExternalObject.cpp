#include <cstdio>
#include "ExternalObject.h"
#include "JSUtils.h"
#include "IPCClient.h"

JSClassRef ExternalObject::class_def;

JSObjectRef ExternalObject::create(JSContextRef ctx, ExternalObjectPageData* page) {
    ExternalObject* obj = new ExternalObject();
    obj->page = page;
    return JSObjectMake(ctx, get_class_def(), obj);
}

void ExternalObject::js_finalize(JSObjectRef obj) {
    delete (ExternalObject*) JSObjectGetPrivate(obj);
}

JSValueRef ExternalObject::js_property(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                       const JSValueRef* argv, JSValueRef* exception) {
    ExternalObject* object = (ExternalObject*) JSObjectGetPrivate(obj);
    if (argc == 2 && JSValueIsString(ctx, argv[0]) && JSValueIsString(ctx, argv[1])) {
        std::string val = object->page->properties[JSUtils::get_js_string(ctx, argv[0])] = JSUtils::get_js_string(ctx, argv[1]);
        return JSValueMakeUndefined(ctx);
    } else {
        auto val = object->page->properties[JSUtils::get_js_string(ctx, argv[0])];
        return JSUtils::new_js_string_value(ctx, val);
    }
}

JSValueRef ExternalObject::js_final_next(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                         JSValueRef const* argv, JSValueRef* exception) {
    ExternalObject* object = (ExternalObject*) JSObjectGetPrivate(obj);
    IPCClient::instance->on_final_next(object->page->page_id, object->page->properties);
    return JSValueMakeUndefined(ctx);
}

JSValueRef ExternalObject::js_final_back(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                         JSValueRef const* argv, JSValueRef* exception) {
    ExternalObject* object = (ExternalObject*) JSObjectGetPrivate(obj);
    IPCClient::instance->on_final_back(object->page->page_id);
    return JSValueMakeUndefined(ctx);
}

JSClassRef ExternalObject::get_class_def() {
    if (class_def)
        return class_def;

    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;

    static JSStaticFunction s_funcs[] = {
            { "Property", &ExternalObject::js_property, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "FinalNext", &ExternalObject::js_final_next, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { "FinalBack", &ExternalObject::js_final_back, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { nullptr, nullptr, 0 }
    };

    classDefinition.className = "Filesystem";
    classDefinition.attributes = kJSClassAttributeNone;
    classDefinition.staticFunctions = s_funcs;
    classDefinition.staticValues = nullptr;
    classDefinition.finalize = &ExternalObject::js_finalize;
    classDefinition.callAsConstructor = nullptr;

    class_def = JSClassCreate(&classDefinition);
    return class_def;
}