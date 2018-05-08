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

void IPCClient::on_final_next(std::map<std::string, std::string> const& map) {
    using Map = std::map<Glib::ustring, Glib::ustring>;
    using MapVariant = Glib::Variant<Map>;

    Map cmap;
    for (auto const& p : map)
        cmap[p.first] = p.second;
    MapVariant variant (MapVariant::create(cmap));
    proxy->call("FinalNext", Glib::VariantContainerBase::create_tuple(variant));
}

void IPCClient::on_final_back() {
    proxy->call("FinalBack");
}