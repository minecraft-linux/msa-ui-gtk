#pragma once

#include <giomm/dbusconnection.h>
#include <giomm/dbusproxy.h>

class IPCClient {

private:
    Glib::RefPtr<Gio::DBus::Connection> connection;
    Glib::RefPtr<Gio::DBus::Proxy> proxy;

public:
    static std::unique_ptr<IPCClient> instance;

    IPCClient(Glib::ustring const& target);

    void on_page_loaded();

    void on_final_next(std::map<std::string, std::string> const& map);

    void on_final_back();

};