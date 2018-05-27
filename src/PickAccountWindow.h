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

    bool has_succeeded() const {
        return !result_cid.empty();
    }

    std::string const& get_selected_cid() const {
        return result_cid;
    }

private:
    Gtk::HeaderBar header_bar;
    Gtk::Button header_cancel;
    Gtk::Button header_pick;

    Gtk::ScrolledWindow scrolled_window;
    Gtk::ListBox list_box;

    std::string result_cid;

    void on_select_done();

    void on_cancelled();

};

class PickAccountRow : public Gtk::ListBoxRow {

public:
    const std::string cid;

    PickAccountRow(PickAccountWindow::Entry const& entry);

private:
    Gtk::Label label;

};