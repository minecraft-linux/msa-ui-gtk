#include "WebLoginWindow.h"
#include "ExtensionIPCServer.h"

WebLoginWindow::WebLoginWindow(ExtensionIPCServer& ipc_server, std::string const& url) {
    set_default_size(480, 640);

    set_modal(true);
    set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);

    auto ctx = WebContext::create();
    ctx->set_process_model(WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS);
    ctx->set_web_extensions_directory("extension/");
    auto variant = Glib::Variant<Glib::ustring>::create(ipc_server.get_unique_name());
    ctx->set_web_extensions_initialization_user_data(variant);
    web_view = Glib::RefPtr<WebView>(new WebView(ctx));

    spinner.set_valign(Gtk::ALIGN_CENTER);
    spinner.set_halign(Gtk::ALIGN_CENTER);

    spinner.show();
    spinner.start();

    web_view->load_uri(url);
    web_view->show();

    stack.add(spinner);
    stack.add(*web_view.get());
    add(stack);

    stack.set_visible_child(spinner);
    stack.show();
}

void WebLoginWindow::on_page_loaded() {
    stack.set_visible_child(*web_view.get());
}

void WebLoginWindow::on_finished(std::map<Glib::ustring, Glib::ustring> map) {
    result = std::move(map);
    finished = true;
    close();
}

void WebLoginWindow::on_cancelled() {
    close();
}