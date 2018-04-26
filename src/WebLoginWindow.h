#pragma once

#include <gtkmm/window.h>
#include <gtkmm/spinner.h>
#include <gtkmm/stack.h>
#include "WebView.h"

class WebLoginWindow : public Gtk::Window {

public:
    WebLoginWindow();

    void on_page_loaded();

private:
    Gtk::Stack stack;
    Gtk::Spinner spinner;
    WebView web_view;

};