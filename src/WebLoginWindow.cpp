#include "WebLoginWindow.h"

WebLoginWindow::WebLoginWindow() {
    set_default_size(480, 640);

    spinner.set_valign(Gtk::ALIGN_CENTER);
    spinner.set_halign(Gtk::ALIGN_CENTER);

    spinner.show();
    spinner.start();

    web_view.load_uri("https://example.com");
    web_view.show();

    stack.add(spinner);
    stack.add(web_view);
    add(stack);

    stack.set_visible_child(web_view);
    stack.show();
}