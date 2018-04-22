#include "WebView.h"

#include <webkit2/webkit2.h>

WebView::WebView() : Widget(webkit_web_view_new()) {
}

void WebView::load_uri(std::string const& uri) {
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(gobj()), uri.c_str());
}