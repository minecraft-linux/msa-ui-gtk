#include "IPCClient.h"

std::unique_ptr<IPCClient> IPCClient::instance;

IPCClient::IPCClient(Glib::ustring const& target) {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");
    proxy = Gio::DBus::Proxy::create_sync(connection, target, "/io/mrarm/msa/ui/WebExtensionHost",
                                          "io.mrarm.msa.ui.WebExtensionHost");
}

void IPCClient::on_page_loaded(guint64 page_id) {
    proxy->call("PageLoaded", Glib::VariantContainerBase::create_tuple(Glib::Variant<guint64>::create(page_id)));
}

void IPCClient::on_final_next(guint64 page_id, std::map<std::string, std::string> const& map) {
    using Map = std::map<Glib::ustring, Glib::ustring>;
    using MapVariant = Glib::Variant<Map>;

    Map cmap;
    for (auto const& p : map)
        cmap[p.first] = p.second;
    MapVariant variant (MapVariant::create(cmap));
    proxy->call("FinalNext", Glib::VariantContainerBase::create_tuple({Glib::Variant<guint64>::create(page_id), variant}));
}

void IPCClient::on_final_back(guint64 page_id) {
    proxy->call("FinalBack", Glib::VariantContainerBase::create_tuple(Glib::Variant<guint64>::create(page_id)));
}