#include "IPCClient.h"

std::unique_ptr<IPCClient> IPCClient::instance;

IPCClient::IPCClient(Glib::ustring const& target) : target(target) {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");

}