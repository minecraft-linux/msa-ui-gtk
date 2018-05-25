#include "PickAccountWindow.h"

PickAccountWindow::PickAccountWindow(std::vector<Entry> entries) {
    set_default_size(400, 300);

    set_modal(true);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);

    header_bar.set_title("Pick account");
    set_titlebar(header_bar);

    header_cancel.set_label("Cancel");
    header_bar.pack_start(header_cancel);

    header_pick.get_style_context()->add_class("suggested-action");
    header_pick.set_label("Select");
    header_bar.pack_end(header_pick);

    for (auto const& e : entries)
        list_box.add(*Gtk::manage(new PickAccountRow(e)));
    scrolled_window.add(list_box);
    add(scrolled_window);

    show_all_children();
}

PickAccountRow::PickAccountRow(PickAccountWindow::Entry const& entry) {
    label.set_text(entry.username);
    label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    label.set_size_request(-1, 40); // TODO: Is this really the correct way? There should be some other way.
    label.set_margin_start(20);
    label.set_margin_end(20);
    add(label);
}