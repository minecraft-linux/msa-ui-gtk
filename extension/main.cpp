#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>
#include "ExternalObject.h"


static void
page_loaded_callback(WebKitWebPage* web_page, gpointer user_data) {
    printf("Page loaded\n");
}

static void
page_created_callback(WebKitWebExtension* extension, WebKitWebPage* web_page, gpointer user_data) {
    g_signal_connect(web_page, "document-loaded", G_CALLBACK(page_loaded_callback), NULL);
}


static void
window_object_cleared_callback(WebKitScriptWorld* world, WebKitWebPage* web_page, WebKitFrame* frame,
                               gpointer user_data) {
    JSGlobalContextRef ctx;
    ctx = webkit_frame_get_javascript_context_for_script_world(frame, world);
    JSObjectRef global = JSContextGetGlobalObject(ctx);

    JSObjectRef external = JSObjectMake(ctx, ExternalObject::get_class_def(), nullptr);
    JSObjectSetProperty(ctx, global, JSStringCreateWithUTF8CString("external"), external, kJSPropertyAttributeNone, nullptr);
}


extern "C" G_MODULE_EXPORT void
webkit_web_extension_initialize(WebKitWebExtension* extension) {
    g_print("webkit_web_extension_initialize\n");
    g_signal_connect (extension, "page-created",
                      G_CALLBACK(page_created_callback), NULL);
    g_signal_connect (webkit_script_world_get_default(), "window-object-cleared",
                      G_CALLBACK(window_object_cleared_callback), NULL);
}