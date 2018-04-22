#pragma once

#include <gtkmm/widget.h>

class WebView : public Gtk::Widget {

public:
    WebView();

    void load_uri(std::string const& uri);

};


