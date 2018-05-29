#include "LoginIPCService.h"
#include "WebLoginWindow.h"
#include "PickAccountWindow.h"
#include "UIThreadExecutor.h"

LoginIPCService::LoginIPCService(Gtk::Application& app, UIThreadExecutor& executor, const std::string& path,
                                 ExtensionIPCServer& extension_ipc_server,
                                 daemon_utils::shutdown_policy shutdown_policy) :
        app(app), executor(executor), auto_shutdown_service(path, shutdown_policy),
        extension_ipc_server(extension_ipc_server) {
    using namespace std::placeholders;
    add_handler("msa/ui/exit", std::bind(&LoginIPCService::handle_exit, this));
    add_handler_async("msa/ui/pick_account", std::bind(&LoginIPCService::handle_pick_account, this, _3, _4));
    add_handler_async("msa/ui/open_browser", std::bind(&LoginIPCService::handle_open_browser, this, _3, _4));

    has_app_ref = true;
    app.hold();
}

void LoginIPCService::request_stop() {
    auto_shutdown_service::request_stop();
    if (has_app_ref) {
        app.release();
        has_app_ref = false;
    }
}

simpleipc::rpc_json_result LoginIPCService::handle_exit() {
    request_stop();
    return simpleipc::rpc_json_result::response(nullptr);
}

void LoginIPCService::handle_pick_account(nlohmann::json const& data, rpc_handler::result_handler const& handler) {
    std::vector<PickAccountWindow::Entry> entries;
    for (auto const& e : data.at("accounts"))
        entries.push_back({e["cid"], e["username"]});

    executor.run([this, entries, handler]() {
        PickAccountWindow* pick_window = new PickAccountWindow(entries);
        pick_window->signal_hide().connect([this, handler, pick_window]() {
            if (pick_window->has_succeeded()) {
                if (pick_window->has_selected_add_account())
                    handler(simpleipc::rpc_json_result::response({{"add_account", true}}));
                else
                    handler(simpleipc::rpc_json_result::response({{"cid", pick_window->get_selected_cid()}}));
            } else {
                handler(simpleipc::rpc_json_result::error(-501, "Operation cancelled by user"));
            }
            delete pick_window;
        });
        pick_window->show();
    });
}

void LoginIPCService::handle_open_browser(nlohmann::json const& data, rpc_handler::result_handler const& handler) {
    std::string url = data["url"];

    executor.run([this, url, handler]() {
        WebLoginWindow* login_window = new WebLoginWindow(extension_ipc_server, url);
        login_window->signal_hide().connect([this, handler, login_window]() {
            if (login_window->has_succeeded()) {
                nlohmann::json res;
                auto& prop = res["properties"] = nlohmann::json::object();
                for (auto const& p : login_window->get_result())
                    prop[p.first] = p.second;
                handler(simpleipc::rpc_json_result::response(res));
            } else {
                handler(simpleipc::rpc_json_result::error(-501, "Operation cancelled by user"));
            }
            extension_ipc_server.remove_window(login_window);
            delete login_window;
        });
        extension_ipc_server.add_window(login_window);
        login_window->show();
    });
}