#include <webkit2/webkit-web-extension.h>

extern "C" G_MODULE_EXPORT void
webkit_web_extension_initialize(WebKitWebExtension* extension) {
    g_print("webkit_web_extension_initialize\n");
}