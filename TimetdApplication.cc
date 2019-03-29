#include "TimetdApplication.h"
#include <iostream>
#include <exception>

TimetdApplication::TimetdApplication()
: Gtk::Application("com.TimeToDo", Gio::APPLICATION_HANDLES_OPEN)
{
}

Glib::RefPtr<TimetdApplication> TimetdApplication::create()
{
  return Glib::RefPtr<TimetdApplication>(new TimetdApplication());
}

TimetdAppWindow* TimetdApplication::create_appwindow()
{
  auto appwindow = TimetdAppWindow::create();

  // Make sure application runs as long as this window is still open.
  add_window(*appwindow);

  // Gtk::Application::add_window() connects a signal handler to the window's
  // signal_hide(). That handler removes the window from the application.
  // If it's the last window to be removed, the application stops running.
  // Gtk::Window::set_application() does not connect a signal handler, but is
  // otherwise equivalent to Gtk::Application::add_window().

  // Delete the window when it is hidden.
  appwindow->signal_hide().connect(sigc::bind(sigc::mem_fun(*this,
    &TimetdApplication::on_hide_window), appwindow));

  return appwindow;
}

void TimetdApplication::on_startup()
{
  // Call the base class's implementation.
  Gtk::Application::on_startup();

  // Add actions and keyboard accelerators for the application menu.
  add_action("preferences", sigc::mem_fun(*this, &TimetdApplication::on_action_preferences));
  add_action("quit", sigc::mem_fun(*this, &TimetdApplication::on_action_quit));
  set_accel_for_action("app.quit", "<Ctrl>Q");

  auto refBuilder = Gtk::Builder::create();
  try
  {
    refBuilder->add_from_resource("/org/gtkmm/timetodo/app_menu.glade");
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "TimetdApplication::on_startup(): " << ex.what() << std::endl;
    return;
  }

  auto object = refBuilder->get_object("appmenu");
  auto app_menu = Glib::RefPtr<Gio::MenuModel>::cast_dynamic(object);
  if (app_menu)
    set_app_menu(app_menu);
  else
    std::cerr << "TimetdApplication::on_startup(): No \"appmenu\" object in app_menu.glade"
              << std::endl;
}

void TimetdApplication::on_activate()
{
  try
  {
    // Application started, show a window.
    auto appwindow = create_appwindow();
    appwindow->present();

    mainWindow = appwindow;
  }

  // If create_appwindow() throws an exception,
  // no window has been created or added to application,
  // application will stop running.
  catch (const Glib::Error& ex)
  {
    std::cerr << "TimetdApplication::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "TimetdApplication::on_activate(): " << ex.what() << std::endl;
  }
}

void TimetdApplication::on_hide_window(Gtk::Window* window)
{
  delete window;
}

void TimetdApplication::on_action_preferences()
{
  try
  {
    auto prefs_dialog = TimetdAppPrefs::create(*get_active_window());
    prefs_dialog->present();

    prefs_dialog->signal_hide().connect(sigc::bind(sigc::mem_fun(mainWindow, &TimetdAppWindow::on_hide_window), prefs_dialog));

    // Delete the dialog when it is hidden.
    prefs_dialog->signal_hide().connect(sigc::bind(sigc::mem_fun(*this,
      &TimetdApplication::on_hide_window), prefs_dialog));
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "TimetdApplication::on_action_preferences(): Glib error, " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "TimetdApplication::on_action_preferences(): std exception" << ex.what() << std::endl;
  }
}

void TimetdApplication::on_action_quit()
{
  // Gio::Application::quit() will make Gio::Application::run() return,
  // it's crude way ending program. The window not removed from application.
  // Window and application destructors not called, because of remaining reference
  // counts in both of them. If we want the destructors to be called,
  // hide the window to remove window from application.
  auto windows = get_windows();
  for (auto window : windows)
    window->hide();

  // Not really necessary, when Gtk::Widget::hide() is called, unless
  // Gio::Application::hold() has been called without a corresponding call
  // to Gio::Application::release().
  quit();
}
