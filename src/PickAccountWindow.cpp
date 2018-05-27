#include "PickAccountWindow.h"

PickAccountWindow::PickAccountWindow(std::vector<Entry> entries) {
    set_default_size(400, 300);

    set_modal(true);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);

    header_bar.set_title("Pick account");
    header_bar.set_show_close_button(true);
    set_titlebar(header_bar);

    list_box.set_selection_mode(Gtk::SELECTION_NONE);
    for (auto const& e : entries)
        list_box.add(*Gtk::manage(new PickAccountRow(e)));
    list_box.add(add_row);
    scrolled_window.add(list_box);
    add(scrolled_window);

    list_box.signal_row_activated().connect(sigc::mem_fun(this, &PickAccountWindow::on_select_item));

    show_all_children();
}

void PickAccountWindow::on_select_item(Gtk::ListBoxRow* row) {
    if (row == &add_row) {
        result_add_account = true;
    } else {
        result_cid = dynamic_cast<PickAccountRow*>(row)->cid;
    }
    close();
}

PickAccountRow::PickAccountRow(PickAccountWindow::Entry const& entry) : cid(entry.cid) {
    label.set_text(entry.username);
    label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    label.set_margin_top(10);
    label.set_margin_bottom(10);
    label.set_margin_start(20);
    label.set_margin_end(20);
    add(label);
}

AddAccountRow::AddAccountRow() {
    label.set_text("Add new account");
    label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    label.set_margin_top(10);
    label.set_margin_bottom(10);
    label.set_margin_start(20);
    label.set_margin_end(20);
    add(label);
}