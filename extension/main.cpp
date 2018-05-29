#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>
#include <glibmm/ustring.h>
#include <glibmm/variant.h>
#include <giomm/init.h>
#include "ExternalObject.h"
#include "IPCClient.h"


static void
page_loaded_callback(WebKitWebPage* web_page, gpointer user_data) {
    IPCClient::instance->on_page_loaded(webkit_web_page_get_id(web_page));
}

static void
page_created_callback(WebKitWebExtension* extension, WebKitWebPage* web_page, gpointer user_data) {
    auto page_data = new ExternalObjectPageData(webkit_web_page_get_id(web_page));
    g_object_set_data_full(G_OBJECT(web_page), "external-page_data", page_data, [](gpointer data) {
        delete (ExternalObjectPageData*) data;
    });
    g_signal_connect(web_page, "document-loaded", G_CALLBACK(page_loaded_callback), NULL);
}


static void
window_object_cleared_callback(WebKitScriptWorld* world, WebKitWebPage* web_page, WebKitFrame* frame, gpointer user_data) {
    JSGlobalContextRef ctx;
    ctx = webkit_frame_get_javascript_context_for_script_world(frame, world);
    JSObjectRef global = JSContextGetGlobalObject(ctx);

    ExternalObjectPageData* data = (ExternalObjectPageData*) g_object_get_data(G_OBJECT(web_page),"external-page_data");
    JSObjectRef external = ExternalObject::create(ctx, data);
    JSObjectSetProperty(ctx, global, JSStringCreateWithUTF8CString("external"), external, kJSPropertyAttributeNone, nullptr);
}


extern "C" G_MODULE_EXPORT void
webkit_web_extension_initialize_with_user_data(WebKitWebExtension* extension, GVariant* user_data) {
    Gio::init();

    {
        gsize str_len;
        auto str = g_variant_get_string(user_data, &str_len);
        printf("dbus addr = %s\n", str);
        IPCClient::instance = std::make_unique<IPCClient>(Glib::ustring(str, str_len));
    }

    g_signal_connect (extension, "page-created",
                      G_CALLBACK(page_created_callback), NULL);
    g_signal_connect (webkit_script_world_get_default(), "window-object-cleared",
                      G_CALLBACK(window_object_cleared_callback), NULL);
}