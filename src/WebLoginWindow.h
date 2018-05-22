#pragma once

#include <gtkmm/window.h>
#include <gtkmm/spinner.h>
#include <gtkmm/stack.h>
#include "WebView.h"

class WebLoginWindow : public Gtk::Window {

public:
    WebLoginWindow(std::string const& url);

    void on_page_loaded();

    void on_finished(std::map<Glib::ustring, Glib::ustring> map);

    void on_cancelled();

    bool has_succeeded() const {
        return finished;
    }

    inline std::map<Glib::ustring, Glib::ustring> const& get_result() const {
        return result;
    };

private:
    Gtk::Stack stack;
    Gtk::Spinner spinner;
    WebView web_view;

    bool finished = false;
    std::map<Glib::ustring, Glib::ustring> result;

};