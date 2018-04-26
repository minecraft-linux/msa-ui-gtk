#include "IPCServer.h"
#include <gio/gio.h>
#include <string>
#include <giomm.h>
#include "WebLoginWindow.h"

Glib::ustring const IPCServer::introspection_xml =
        "<node>\n"
        "   <interface name='io.mrarm.msa.ui.WebExtensionHost'>\n"
        "       <method name='PageLoaded' />\n"
        "   </interface>\n"
        "</node>";
Glib::RefPtr<Gio::DBus::NodeInfo> IPCServer::introspection_data;

IPCServer::IPCServer() : interface_vtable(sigc::mem_fun(*this, &IPCServer::on_method_call)) {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");
    if (!introspection_data)
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(introspection_xml);
    registeration_id = connection->register_object("/io/mrarm/msa/ui/WebExtensionHost",
                                                   introspection_data->lookup_interface(), interface_vtable);
}

void IPCServer::on_method_call(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&, const Glib::ustring&,
                               const Glib::ustring&, const Glib::ustring& method_name,
                               const Glib::VariantContainerBase& parameters,
                               const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation) {
    if (method_name == "PageLoaded") {
        if (window != nullptr)
            window->on_page_loaded();
    } else {
        Gio::DBus::Error error(Gio::DBus::Error::UNKNOWN_METHOD, "Method does not exist.");
        invocation->return_error(error);
    }
}