#pragma once

#include <giomm/dbusconnection.h>

class IPCClient {

private:
    Glib::RefPtr<Gio::DBus::Connection> connection;
    Glib::ustring target;

public:
    static std::unique_ptr<IPCClient> instance;

    IPCClient(Glib::ustring const& target);

};