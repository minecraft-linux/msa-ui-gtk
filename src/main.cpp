#include <gtkmm.h>
#include <webkit2/webkit2.h>
#include "WebLoginWindow.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "io.mrarm.msa.ui");

    auto ctx = webkit_web_context_get_default();
    webkit_web_context_set_process_model(ctx, WEBKIT_PROCESS_MODEL_SHARED_SECONDARY_PROCESS);
    webkit_web_context_set_web_extensions_directory(ctx, "extension/");

    WebLoginWindow window;

    return app->run(window);
}