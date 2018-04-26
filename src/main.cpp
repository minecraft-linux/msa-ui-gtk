#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "WebLoginWindow.h"
#include "IPCServer.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "io.mrarm.msa.ui", Gio::APPLICATION_NON_UNIQUE);
    IPCServer ipc_server;

    auto ctx = webkit_web_context_get_default();
    webkit_web_context_set_process_model(ctx, WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS);
    webkit_web_context_set_web_extensions_directory(ctx, "extension/");
    auto variant = Glib::Variant<Glib::ustring>::create(ipc_server.get_unique_name());
    webkit_web_context_set_web_extensions_initialization_user_data(ctx, variant.gobj());

    WebLoginWindow window;
    ipc_server.set_window(&window);

    return app->run(window);
}