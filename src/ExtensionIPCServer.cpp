#include "ExtensionIPCServer.h"
#include <gio/gio.h>
#include <string>
#include <giomm.h>
#include "WebLoginWindow.h"

Glib::ustring const ExtensionIPCServer::introspection_xml =
        "<node>\n"
        "   <interface name='io.mrarm.msa.ui.WebExtensionHost'>\n"
        "       <method name='PageLoaded' />\n"
        "       <method name='FinalNext'>\n"
        "           <arg name='data' direction='in' type='a{ss}' />\n"
        "       </method>\n"
        "       <method name='FinalBack' />\n"
        "   </interface>\n"
        "</node>";
Glib::RefPtr<Gio::DBus::NodeInfo> ExtensionIPCServer::introspection_data;

ExtensionIPCServer::ExtensionIPCServer() : interface_vtable(sigc::mem_fun(*this, &ExtensionIPCServer::on_method_call)) {
    connection = Gio::DBus::Connection::get_sync(Gio::DBus::BUS_TYPE_SESSION);
    if (!connection)
        throw std::runtime_error("Failed to get dbus");
    if (!introspection_data)
        introspection_data = Gio::DBus::NodeInfo::create_for_xml(introspection_xml);
    registeration_id = connection->register_object("/io/mrarm/msa/ui/WebExtensionHost",
                                                   introspection_data->lookup_interface(), interface_vtable);
}

void ExtensionIPCServer::on_method_call(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&, const Glib::ustring&,
                               const Glib::ustring&, const Glib::ustring& method_name,
                               const Glib::VariantContainerBase& parameters,
                               const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation) {
    if (method_name == "PageLoaded") {
        if (window != nullptr)
            window->on_page_loaded();
    } else if (method_name == "FinalNext") {
        Glib::Variant<std::map<Glib::ustring, Glib::ustring>> params;
        parameters.get_child(params, 0);
        printf("FinalNext %s\n", params.print(true).c_str());
        if (window != nullptr)
            window->on_finished(params.get());
    } else if (method_name == "FinalBack") {
        if (window != nullptr)
            window->on_cancelled();
    } else {
        Gio::DBus::Error error(Gio::DBus::Error::UNKNOWN_METHOD, "Method does not exist.");
        invocation->return_error(error);
    }
}