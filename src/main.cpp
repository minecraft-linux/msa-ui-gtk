#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "WebLoginWindow.h"
#include "ExtensionIPCServer.h"
#include "LoginIPCService.h"
#include "UIThreadExecutor.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "io.mrarm.msa.ui", Gio::APPLICATION_NON_UNIQUE);
    app->add_main_option_entry(Gtk::Application::OPTION_TYPE_FILENAME, "path", 'p', "Service path");
    app->add_main_option_entry(Gtk::Application::OPTION_TYPE_BOOL, "auto-exit", 'x', "Auto-exit the service");
    UIThreadExecutor executor;
    ExtensionIPCServer extension_ipc_server;

    auto ctx = webkit_web_context_get_default();
    webkit_web_context_set_process_model(ctx, WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS);
    webkit_web_context_set_web_extensions_directory(ctx, "extension/");
    auto variant = Glib::Variant<Glib::ustring>::create(extension_ipc_server.get_unique_name());
    webkit_web_context_set_web_extensions_initialization_user_data(ctx, variant.gobj());

    std::shared_ptr<LoginIPCService> login_service;

    app->signal_handle_local_options().connect([&app, &executor, &login_service, &extension_ipc_server]
                                                       (const Glib::RefPtr<Glib::VariantDict>& cmd) {
        std::string path;
        bool auto_exit;
        cmd->lookup_value("path", path);
        cmd->lookup_value("auto-exit", auto_exit);
        auto mode = auto_exit ? daemon_utils::shutdown_policy::no_connections : daemon_utils::shutdown_policy::never;
        login_service = std::shared_ptr<LoginIPCService>(new LoginIPCService(*app.get(), executor, path,
                                                                             extension_ipc_server, mode));
        return 0;
    });

    return app->run();
}