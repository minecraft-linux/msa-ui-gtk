#pragma once

#include <simpleipc/server/service.h>
#include <gtkmm/application.h>
#include <giomm.h>
#include "ExtensionIPCServer.h"

class UIThreadExecutor;

class LoginIPCService : simpleipc::server::service {

private:
    Gtk::Application& app;
    UIThreadExecutor& executor;
    Glib::Dispatcher dispatcher;
    ExtensionIPCServer& extension_ipc_server;
    bool has_app_ref = false;

public:
    LoginIPCService(Gtk::Application& app, UIThreadExecutor& executor, const std::string& path,
                    ExtensionIPCServer& extension_ipc_server);

    simpleipc::rpc_json_result handle_exit();

    void handle_open_browser(nlohmann::json const& data, rpc_handler::result_handler const& handler);

};