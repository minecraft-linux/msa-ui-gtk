#pragma once

#include <giomm/dbusconnection.h>

class WebLoginWindow;

class IPCServer {

private:
    static Glib::ustring const introspection_xml;
    static Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;

    Glib::RefPtr<Gio::DBus::Connection> connection;
    Gio::DBus::InterfaceVTable const interface_vtable;
    guint registeration_id;
    WebLoginWindow* window = nullptr;

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&, const Glib::ustring&,
                        const Glib::ustring&, const Glib::ustring& method_name,
                        const Glib::VariantContainerBase& parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);

public:
    IPCServer();

    void set_window(WebLoginWindow* window) {
        this->window = window;
    }

    Glib::ustring get_unique_name() const {
        return connection->get_unique_name();
    }

};