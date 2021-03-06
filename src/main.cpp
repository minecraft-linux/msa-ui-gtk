#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "WebLoginWindow.h"
#include "ExtensionIPCServer.h"
#include "LoginIPCService.h"
#include "UIThreadExecutor.h"
#include "MsaDaemonManager.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "io.mrarm.msa.ui", Gio::APPLICATION_NON_UNIQUE);
    app->add_main_option_entry(Gtk::Application::OPTION_TYPE_FILENAME, "path", 'p', "Service path");
    app->add_main_option_entry(Gtk::Application::OPTION_TYPE_BOOL, "auto-exit", 'x', "Auto-exit the service");
    app->add_main_option_entry(Gtk::Application::OPTION_TYPE_FILENAME, "msa-path", 'm', "Path to the MSA service");
    UIThreadExecutor executor;
    ExtensionIPCServer extension_ipc_server;

    std::shared_ptr<LoginIPCService> login_service;

    app->signal_handle_local_options().connect([&app, &executor, &login_service, &extension_ipc_server]
                                                       (const Glib::RefPtr<Glib::VariantDict>& cmd) {
        std::string path, msa_path;
        bool auto_exit;
        cmd->lookup_value("path", path);
        cmd->lookup_value("auto-exit", auto_exit);
        cmd->lookup_value("msa-path", msa_path);
        MsaDaemonManager::instance.set_msa_path(msa_path);
        auto mode = auto_exit ? daemon_utils::shutdown_policy::no_connections : daemon_utils::shutdown_policy::never;
        login_service = std::shared_ptr<LoginIPCService>(new LoginIPCService(*app.get(), executor, path,
                                                                             extension_ipc_server, mode));
        return 0;
    });

    return app->run();
}