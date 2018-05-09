#include "WebLoginWindow.h"

WebLoginWindow::WebLoginWindow(std::string const& url) {
    set_default_size(480, 640);

    spinner.set_valign(Gtk::ALIGN_CENTER);
    spinner.set_halign(Gtk::ALIGN_CENTER);

    spinner.show();
    spinner.start();

    web_view.load_uri(url);
    web_view.show();

    stack.add(spinner);
    stack.add(web_view);
    add(stack);

    stack.set_visible_child(spinner);
    stack.show();
}

void WebLoginWindow::on_page_loaded() {
    stack.set_visible_child(web_view);
}