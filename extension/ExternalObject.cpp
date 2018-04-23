#include <cstdio>
#include "ExternalObject.h"
#include "JSUtils.h"

JSClassRef ExternalObject::class_def;
std::map<std::string, std::string> ExternalObject::properties;

JSValueRef ExternalObject::js_property(JSContextRef ctx, JSObjectRef func, JSObjectRef obj, size_t argc,
                                       const JSValueRef* argv, JSValueRef* exception) {
    if (argc == 2 && JSValueIsString(ctx, argv[0]) && JSValueIsString(ctx, argv[1])) {
        printf("Assign property\n");
        std::string val = properties[JSUtils::get_js_string(ctx, argv[0])] = JSUtils::get_js_string(ctx, argv[1]);
        return JSValueMakeUndefined(ctx);
    } else {
        auto val = properties[JSUtils::get_js_string(ctx, argv[0])];
        return JSUtils::new_js_string_value(ctx, val);
    }
    return JSValueMakeUndefined(ctx);
}

JSClassRef ExternalObject::get_class_def() {
    if (class_def)
        return class_def;

    JSClassDefinition classDefinition = kJSClassDefinitionEmpty;

    static JSStaticFunction s_funcs[] = {
            { "Property", &ExternalObject::js_property, kJSPropertyAttributeReadOnly | kJSPropertyAttributeDontDelete },
            { nullptr, nullptr, 0 }
    };

    classDefinition.className = "Filesystem";
    classDefinition.attributes = kJSClassAttributeNone;
    classDefinition.staticFunctions = s_funcs;
    classDefinition.staticValues = nullptr;
    classDefinition.finalize = nullptr;
    classDefinition.callAsConstructor = nullptr;

    class_def = JSClassCreate(&classDefinition);
    return class_def;
}