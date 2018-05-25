#pragma once

#include <daemon_utils/auto_shutdown_service.h>
#include <gtkmm/application.h>
#include <giomm.h>
#include "ExtensionIPCServer.h"

class UIThreadExecutor;

class LoginIPCService : public daemon_utils::auto_shutdown_service {

private:
    Gtk::Application& app;
    UIThreadExecutor& executor;
    ExtensionIPCServer& extension_ipc_server;
    bool has_app_ref = false;

protected:
    void request_stop() override;

public:
    LoginIPCService(Gtk::Application& app, UIThreadExecutor& executor, const std::string& path,
                    ExtensionIPCServer& extension_ipc_server, daemon_utils::shutdown_policy shutdown_policy);

    simpleipc::rpc_json_result handle_exit();

    void handle_open_browser(nlohmann::json const& data, rpc_handler::result_handler const& handler);

};