#include <gio/gio.h>
#include <string>
#include "IPCServer.h"

IPCServer::IPCServer() {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");
}