#include "WebView.h"

#include <webkit2/webkit2.h>

WebContext::WebContext() : Object(G_OBJECT(webkit_web_context_new())) {
}

void WebContext::set_process_model(WebKitProcessModel model) {
    webkit_web_context_set_process_model(WEBKIT_WEB_CONTEXT(gobj()), model);
}

void WebContext::set_web_extensions_directory(std::string const& dir) {
    webkit_web_context_set_web_extensions_directory(WEBKIT_WEB_CONTEXT(gobj()), dir.c_str());
}

void WebContext::set_web_extensions_initialization_user_data(Glib::VariantBase& variant) {
    webkit_web_context_set_web_extensions_initialization_user_data(WEBKIT_WEB_CONTEXT(gobj()), variant.gobj());
}

WebView::WebView() : Widget(webkit_web_view_new()) {
}

WebView::WebView(Glib::RefPtr<WebContext> ctx) : Widget(
        webkit_web_view_new_with_context(WEBKIT_WEB_CONTEXT(ctx->gobj()))) {
}

guint64 WebView::get_page_id() const {
    return webkit_web_view_get_page_id(WEBKIT_WEB_VIEW(gobj()));
}

void WebView::load_uri(std::string const& uri) {
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(gobj()), uri.c_str());
}