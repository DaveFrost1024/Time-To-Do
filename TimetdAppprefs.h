#ifndef TIMETDAPPPREFS_H
#define TIMETDAPPPREFS_H

#include <gtkmm.h>
#include "TimetdAppwindow.h"

class TimetdAppPrefs : public Gtk::Dialog
{
public:
    TimetdAppPrefs(BaseObjectType* cobject,
        const Glib::RefPtr<Gtk::Builder>& refBuilder);

    static TimetdAppPrefs* create(Gtk::Window& parent);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gio::Settings> m_settings;
    Gtk::CheckButton* m_centisecond;

    Gtk::Entry* m_averageLife;
    Gtk::Entry* m_birthYear;
    Gtk::Entry* m_birthMonth;
    Gtk::Entry* m_birthDay;

};

#endif /* TIMETDAPPPREFS_H_ */
