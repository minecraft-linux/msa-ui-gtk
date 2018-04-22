#include <gtkmm.h>
#include "WebLoginWindow.h"

int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "io.mrarm.msa.ui");

    WebLoginWindow window;

    return app->run(window);
}