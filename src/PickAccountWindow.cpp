#include "PickAccountWindow.h"
#include "ProfilePictureManager.h"

PickAccountWindow::PickAccountWindow(std::vector<Entry> entries) {
    set_default_size(400, 300);

    set_modal(true);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);

    header_bar.set_title("Pick account");
    header_bar.set_show_close_button(true);
    set_titlebar(header_bar);

    list_box.set_selection_mode(Gtk::SELECTION_NONE);
    auto default_profile_image = Gdk::Pixbuf::create_from_resource("/app/default_profile.svg");
    for (auto const& e : entries)
        list_box.add(*Gtk::manage(new PickAccountRow(e, default_profile_image)));
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

PickAccountRow::PickAccountRow(PickAccountWindow::Entry const& entry, Glib::RefPtr<Gdk::Pixbuf> default_profile_image)
        : cid(entry.cid) {
    image = Glib::RefPtr<Gtk::Image>(new Gtk::Image());
    image->set_size_request(32, 32);

    std::string image_url = "https://storage.live.com/users/0x" + entry.cid + "/myprofile/expressionprofile/profilephoto:UserTileStatic";
    auto image_ptr = image;
    image->set(default_profile_image);
    ProfilePictureManager::instance.get_image_async(entry.cid, image_url, [image_ptr](Glib::RefPtr<Gdk::Pixbuf> image_data) {
        image_ptr->set(image_data);
    });

    box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box.set_border_width(8);
    box.set_spacing(8);

    box.pack_start(*image.get(), Gtk::PACK_SHRINK);

    label.set_text(entry.username);
    label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    box.pack_start(label);

    add(box);
}

AddAccountRow::AddAccountRow() {
    box.set_orientation(Gtk::ORIENTATION_HORIZONTAL);
    box.set_border_width(8);
    box.set_spacing(8);

    icon.set_padding(8, 0);
    icon.set_from_icon_name("list-add", Gtk::ICON_SIZE_BUTTON);
    box.pack_start(icon, Gtk::PACK_SHRINK);

    label.set_text("Add a new account");
    label.set_alignment(Gtk::ALIGN_START, Gtk::ALIGN_CENTER);
    box.pack_start(label);

    add(box);
}