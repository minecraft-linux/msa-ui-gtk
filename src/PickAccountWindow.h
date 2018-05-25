#pragma once

#include <gtkmm/window.h>
#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/headerbar.h>
#include "WebView.h"

class PickAccountRow;

class PickAccountWindow : public Gtk::Window {

public:
    struct Entry {
        std::string cid;
        std::string username;
    };

    PickAccountWindow(std::vector<Entry> entries);

private:
    Gtk::HeaderBar header_bar;
    Gtk::Button header_cancel;
    Gtk::Button header_pick;

    Gtk::ScrolledWindow scrolled_window;
    Gtk::ListBox list_box;

};

class PickAccountRow : public Gtk::ListBoxRow {

public:
    PickAccountRow(PickAccountWindow::Entry const& entry);

private:
    Gtk::Label label;

};