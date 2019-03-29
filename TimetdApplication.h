#ifndef TIMETDAPPLICATION_H
#define TIMETDAPPLICATION_H

#include <gtkmm.h>
#include "TimetdAppwindow.h"
#include "TimetdAppprefs.h"

class TimetdAppWindow;

class TimetdApplication: public Gtk::Application
{
protected:
  TimetdApplication();

public:
  static Glib::RefPtr<TimetdApplication> create();

protected:
  // Override default signal handlers
  void on_startup() override;
  void on_activate() override;

  TimetdAppWindow* mainWindow = nullptr;

private:
  TimetdAppWindow* create_appwindow();
  void on_hide_window(Gtk::Window* window);
  void on_action_preferences();
  void on_action_quit();
};

#endif /* TIMETDAPPLICATION_H */
