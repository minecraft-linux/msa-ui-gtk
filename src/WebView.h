#pragma once

#include <gtkmm/widget.h>
#include <webkit2/webkit2.h>

class WebContext : public Glib::Object {

public:
    WebContext();

    void set_process_model(WebKitProcessModel model);

    void set_web_extensions_directory(std::string const& dir);

    void set_web_extensions_initialization_user_data(Glib::VariantBase& variant);

    static Glib::RefPtr<WebContext> create() {
        return Glib::RefPtr<WebContext>(new WebContext());
    }

};

class WebView : public Gtk::Widget {

public:
    WebView();
    WebView(Glib::RefPtr<WebContext> ctx);

    guint64 get_page_id() const;

    void load_uri(std::string const& uri);

};


