#include "LoginIPCService.h"
#include "WebLoginWindow.h"
#include "UIThreadExecutor.h"

LoginIPCService::LoginIPCService(Gtk::Application& app, UIThreadExecutor& executor, const std::string& path,
                                 ExtensionIPCServer& extension_ipc_server) :
        app(app), executor(executor), service(path), extension_ipc_server(extension_ipc_server) {
    using namespace std::placeholders;
    add_handler("msa/ui/exit", std::bind(&LoginIPCService::handle_exit, this));
    add_handler_async("msa/ui/open_browser", std::bind(&LoginIPCService::handle_open_browser, this, _3, _4));

    has_app_ref = true;
    app.hold();
}

simpleipc::rpc_json_result LoginIPCService::handle_exit() {
    if (has_app_ref) {
        app.release();
        has_app_ref = false;
    }
    return simpleipc::rpc_json_result::response(nullptr);
}

void LoginIPCService::handle_open_browser(nlohmann::json const& data, rpc_handler::result_handler const& handler) {
    std::string url = data["url"];

    executor.run([this, url, handler]() {
        if (extension_ipc_server.has_window()) {
            handler(simpleipc::rpc_json_result::error(100, "A browser operation is already in progress"));
            return;
        }
        WebLoginWindow* login_window = new WebLoginWindow(url);
        login_window->signal_hide().connect([this, login_window]() {
            extension_ipc_server.set_window(nullptr);
            delete login_window;
        });
        extension_ipc_server.set_window(login_window);
        login_window->show();
    });
}