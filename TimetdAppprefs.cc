#include "TimetdAppprefs.h"

#include <iostream>
#include <stdexcept>

TimetdAppPrefs::TimetdAppPrefs(BaseObjectType* cobject,
  const Glib::RefPtr<Gtk::Builder>& refBuilder)
: Gtk::Dialog(cobject),
  m_refBuilder(refBuilder),
  m_settings(),
  m_centisecond(nullptr),
  m_averageLife(nullptr),
  m_birthYear(nullptr),
  m_birthMonth(nullptr),
  m_birthDay(nullptr)
{

  m_refBuilder->get_widget("centisecond", m_centisecond);
  if (!m_centisecond)
    throw std::runtime_error("No \"centisecond\" object in prefs.glade");

  m_refBuilder->get_widget("averageLife", m_averageLife);
  if (!m_averageLife)
    throw std::runtime_error("No \"averageLife\" object in prefs.glade");

  m_refBuilder->get_widget("birthyear", m_birthYear);
  if (!m_birthYear)
    throw std::runtime_error("No \"birthyear\" object in prefs.glade");

  m_refBuilder->get_widget("birthmonth", m_birthMonth);
  if (!m_birthMonth)
    throw std::runtime_error("No \"birthmonth\" object in prefs.glade");  

  m_refBuilder->get_widget("birthday", m_birthDay);
  if (!m_birthDay)
    throw std::runtime_error("No \"birthday\" object in prefs.glade");  

  m_settings = Gio::Settings::create("org.gtkmm.timetodo");
  m_settings->bind("centisecond", m_centisecond->property_active());
  m_settings->bind("average-life-expectancy", m_averageLife->property_text());
  m_settings->bind("birthyear", m_birthYear->property_text());
  m_settings->bind("birthmonth", m_birthMonth->property_text());
  m_settings->bind("birthday", m_birthDay->property_text());
}

//static
TimetdAppPrefs* TimetdAppPrefs::create(Gtk::Window& parent)
{
  // Load the Builder file and instantiate its widgets.
  auto refBuilder = Gtk::Builder::create_from_resource("/org/gtkmm/timetodo/prefs.glade");

  TimetdAppPrefs* dialog = nullptr;
  refBuilder->get_widget_derived("prefs_dialog", dialog);
  if (!dialog)
    throw std::runtime_error("No \"prefs_dialog\" object in prefs.glade");

  dialog->set_transient_for(parent);

  return dialog;
}
