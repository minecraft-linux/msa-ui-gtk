#pragma once

#include <giomm/dbusconnection.h>

class IPCServer {

private:
    Glib::RefPtr<Gio::DBus::Connection> connection;

public:
    IPCServer();

    Glib::ustring get_unique_name() const {
        return connection->get_unique_name();
    }

};