#pragma once

#include <giomm/dbusconnection.h>

class WebLoginWindow;

class ExtensionIPCServer {

private:
    static Glib::ustring const introspection_xml;
    static Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;

    Glib::RefPtr<Gio::DBus::Connection> connection;
    Gio::DBus::InterfaceVTable const interface_vtable;
    guint registeration_id;
    std::map<guint64, WebLoginWindow*> windows;

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection>&, const Glib::ustring&, const Glib::ustring&,
                        const Glib::ustring&, const Glib::ustring& method_name,
                        const Glib::VariantContainerBase& parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation>& invocation);

public:
    ExtensionIPCServer();

    void add_window(WebLoginWindow* window);
    void remove_window(WebLoginWindow* window);
    WebLoginWindow* get_window(guint64 page_id);
    WebLoginWindow* get_window(const Glib::VariantContainerBase& parameters, gsize param = 0);

    Glib::ustring get_unique_name() const {
        return connection->get_unique_name();
    }

};