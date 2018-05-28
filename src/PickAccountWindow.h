#pragma once

#include <gtkmm/window.h>
#include <gtkmm/listbox.h>
#include <gtkmm/label.h>
#include <gtkmm/image.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/headerbar.h>
#include <gtkmm/box.h>
#include "WebView.h"

class PickAccountRow;

class AddAccountRow : public Gtk::ListBoxRow {

public:
    AddAccountRow();

private:
    Gtk::Box box;
    Gtk::Label label;
    Gtk::Image icon;

};

class PickAccountWindow : public Gtk::Window {

public:
    struct Entry {
        std::string cid;
        std::string username;
    };

    PickAccountWindow(std::vector<Entry> entries);

    bool has_succeeded() const {
        return !result_cid.empty() || result_add_account;
    }

    bool has_selected_add_account() const {
        return result_add_account;
    }

    std::string const& get_selected_cid() const {
        return result_cid;
    }

private:
    Gtk::HeaderBar header_bar;

    Gtk::ScrolledWindow scrolled_window;
    Gtk::ListBox list_box;

    AddAccountRow add_row;

    std::string result_cid;
    bool result_add_account = false;

    void on_select_item(Gtk::ListBoxRow* row);

};

class PickAccountRow : public Gtk::ListBoxRow {

public:
    const std::string cid;

    PickAccountRow(PickAccountWindow::Entry const& entry, Glib::RefPtr<Gdk::Pixbuf> default_profile_image);

private:
    Gtk::Box box;
    Gtk::Label label;
    Glib::RefPtr<Gtk::Image> image;

};