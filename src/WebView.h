#pragma once

#include <gtkmm/widget.h>

class WebView : public Gtk::Widget {

public:
    WebView();

    guint64 get_page_id() const;

    void load_uri(std::string const& uri);

};


