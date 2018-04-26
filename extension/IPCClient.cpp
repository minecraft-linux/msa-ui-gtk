#include "IPCClient.h"

std::unique_ptr<IPCClient> IPCClient::instance;

IPCClient::IPCClient(Glib::ustring const& target) {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");
    proxy = Gio::DBus::Proxy::create_sync(connection, target, "/io/mrarm/msa/ui/WebExtensionHost",
                                          "io.mrarm.msa.ui.WebExtensionHost");
}

void IPCClient::on_page_loaded() {
    proxy->call("PageLoaded");
}