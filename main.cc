#include "TimetdApplication.h"

#include <ctime>
#include <iostream>

// To compile gresource:
// glib-compile-resources --target=resources.c --generate-source timetodo.gresource.xml

// To export & compile gschema:
// cp org.gtkmm.timetodo.gschema.xml /home/david/.local/share/glib-2.0/schemas
// export GSETTINGS_SCHEMA_DIR=~/.local/share/glib-2.0/schemas
// glib-compile-schemas /home/david/.local/share/glib-2.0/schemas

int main(int argc, char* argv[])
{
    // Since this example is running uninstalled, we have to help it find its
    // schema. This is *not* necessary in a properly installed application.
    Glib::setenv ("GSETTINGS_SCHEMA_DIR", ".", false);

    auto application = TimetdApplication::create();

    // Start application show window,
    return application->run(argc, argv);
}
